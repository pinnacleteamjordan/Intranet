#include <iostream>
#include <fstream>
#include <regex>

#include <unistd.h>
#include <libpq-fe.h>

#include <python3.6m/Python.h>

#include "crow_all.h"

static void exit_nicely(PGconn *conn) {
    PQfinish(conn);
    exit(1);
}

class SQL {
    private:
        PGconn *conn;

    public:
        SQL() {
            const char *conninfo;

            conninfo = "dbname = WWW_PinnacleWealth_CA";
            conn = PQconnectdb(conninfo);

            if (PQstatus(conn) != CONNECTION_OK) {
                fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
                exit_nicely(conn);
            }
        };

        ~SQL() {
            PGresult *res = PQexec(conn, "CLOSE myportal");
            PQclear(res);
            res = PQexec(conn, "END");
            PQclear(res);
            PQfinish(conn);
        };

        std::map<std::string, std::string> Run(std::string sql) {
            std::map<std::string, std::string> returnData;
            std::regex re("^SELECT.+");
            PGresult *res;
            int nFields, x, i;

            res = PQexec(conn, "BEGIN");
            if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
                PQclear(res);
                exit_nicely(conn);
            }
            PQclear(res);

            if (std::regex_match(sql, re)) {
                std::string command = "DECLARE myportal CURSOR FOR " + sql;
                res = PQexec(conn, command.c_str());
                if (PQresultStatus(res) != PGRES_COMMAND_OK) {
                    fprintf(stderr, "DECLARE CURSOR failed: %s", PQerrorMessage(conn));
                    PQclear(res);
                    exit_nicely(conn);
                }
                PQclear(res);

                res = PQexec(conn, "FETCH ALL in myportal");
                if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                    fprintf(stderr, "FETCH ALL failed: %s", PQerrorMessage(conn));
                    PQclear(res);
                    exit_nicely(conn);
                }
                nFields = PQnfields(res);

                for (x = 0; x < PQntuples(res); x++) {
                    for (i = 0; i < nFields; i++) {
                        std::string str_a = PQfname(res, i);
                        std::string str_b = PQgetvalue(res, x, i);
                        returnData.insert(std::pair<std::string, std::string>(str_a, str_b));
                    }
                }
            } else {
                res = PQexec(conn, sql.c_str());
            }

            PQclear(res);
            return returnData;
        }
};

struct Middleware {
    Middleware() {
    }

    struct context {
    };

    void before_handle(crow::request& req, crow::response& res, context& ctx) {
        std::regex re("^/?static.+");

        if (std::regex_match(req.url, re)) {
            std::ifstream file("." + req.url);
            std::string str;
            std::string append;

            if (file.good()) {
                while(std::getline(file, str)) {
                    append += str;
                    append.push_back('\n');
                }
            }

            res.write(append);
            res.end();
        }
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx) {
    }
};

std::map<std::string, std::string> AnalyticsData() {
    std::map<std::string, std::string> returnData;
    char currPath[256] = "";
    getcwd(currPath, sizeof(currPath));
    std::string path = (const char*) &currPath;
    path += "/pylibs";

    Py_Initialize();
    PyObject* sysPath = PySys_GetObject("path");
    PyObject* uniPath = PyUnicode_FromString(path.c_str());
    PyList_Append(sysPath, uniPath);
    PyImport_ImportModule("threading");
    PyObject* objModule = PyImport_ImportModule("crow_analytics");
    PyObject* objDict = PyModule_GetDict(objModule);
    PyObject* objFunction = PyDict_GetItemString(objDict, "main");

    PyObject* objArgs = PyTuple_New(1);
    PyObject* uniArgs = PyUnicode_FromString("dimension");

    PyTuple_SetItem(objArgs, 0, uniArgs);

    PyObject* pyResult = PyObject_CallObject(objFunction, objArgs);
    // int pyValue = PyList_Size(pyResult);
    Py_Finalize();

    return returnData;
}

int main() {
    crow::App<Middleware> app;
    crow::mustache::set_base("views");

    CROW_ROUTE(app, "/dashboard")([](){
            crow::mustache::context ctx;

            // std::map<std::string, std::string> temp = AnalyticsData();
            ctx["title"] = "Dashboard Home";
            ctx["navigation"] = crow::mustache::load("navigation.html").render();

            return crow::mustache::load("dashboard.html").render(ctx);
            });

    CROW_ROUTE(app, "/dashboard/representatives")([](){
            crow::mustache::context ctx;

            ctx["title"] = "Dashboard Representatives";
            ctx["navigation"] = crow::mustache::load("navigation.html").render();

            return crow::mustache::load("representatives.html").render(ctx);
            });

    CROW_ROUTE(app, "/get/reps/init").methods("GET"_method)([]{
            std::map<std::string, std::string>::iterator mapIterator;
            std::map<std::string, std::string> sqlValue;
            crow::json::wvalue json;

            SQL sql;
            sqlValue = sql.Run("SELECT firstname, lastname, id FROM data_representatives");

            mapIterator = sqlValue.begin();
            for (mapIterator=sqlValue.begin(); mapIterator!=sqlValue.end(); mapIterator++) {
            json[mapIterator->first] = mapIterator->second;
            }

            return json;
            });

    CROW_ROUTE(app, "/get/reps/info").methods("GET"_method)([]{
            std::map<std::string, std::string>::iterator mapIterator;
            std::map<std::string, std::string> sqlValue;
            crow::json::wvalue json;

            SQL sql;
            sqlValue = sql.Run("SELECT * FROM data_representatives");

            mapIterator = sqlValue.begin();
            for (mapIterator=sqlValue.begin(); mapIterator!=sqlValue.end(); mapIterator++) {
            json[mapIterator->first] = mapIterator->second;
            }

            return json;
            });

    app.route_dynamic("/post/reps/info").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
            auto x = crow::json::load(req.body);
            if (!x)
            return crow::response(400);

            std::ostringstream os;

            os << "UPDATE data_representatives SET firstname=" << x["firstname"]
            << ", lastname=" << x["lastname"]
            << ", title=" << x["title"]
            << ", email=" << x["email"]
            << ", phone=" << x["phone"]
            << ", workphone=" << x["workphone"]
            << ", cellphone=" << x["cellphone"]
            << ", faxnumber=" << x["faxnumber"]
            << " AND id=" << x["id"];

            SQL sql;
            sql.Run(os.str());

            crow::json::wvalue json;
            json["status"] = "Complete";
            json["data"] = "Updated user information";

            return crow::response(json);
    });

    app.route_dynamic("/post/reps/new").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
            auto x = crow::json::load(req.body);
            if (!x)
            return crow::response(400);

            std::ostringstream os;

            os << "INSERT INTO data_representatives (firstname, lastname, title, email, phone, workphone, "
            << "cellphone, faxnumber) VALUES (" << x["firstname"]
            << ", " << x["lastname"]
            << ", " << x["title"]
            << ", " << x["email"]
            << ", " << x["phone"]
            << ", " << x["workphone"]
            << ", " << x["cellphone"]
            << ", " << x["faxnumber"];

            crow::json::wvalue json;
            json["status"] = "Complete";
            json["data"] = "Successfully added Representative";

            return crow::response(json);
    });

    app.port(18080).multithreaded().run();
}
