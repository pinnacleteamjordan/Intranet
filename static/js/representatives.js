/* jshint browser: true */

(function () {
    var REPS = {},
        CREATE = {};

    CREATE.li = function (className, id, text) {
        var nodeLI = document.createElement('li'),
            nodeText = document.createTextNode(text);

        if (className) {
            nodeLI.className = className;
        }

        if (id) {
            nodeLI.id = id;
        }

        nodeLI.appendChild(nodeText);
        return nodeLI;
    };

    CREATE.input = function (type, placeholder, classname, append) {
        var nodeInput = document.createElement('input');

        if (type)
            nodeInput.setAttribute('type', type);
        else
            nodeInput.setAttribute('type', 'hidden');
        if (placeholder)
            nodeInput.setAttribute('placeholder', placeholder);
        if (classname)
            nodeInput.setAttribute('class', classname);
        if (append)
            append.appendChild(nodeInput);

        return nodeInput;
    }

    REPS.editId = -1;
    REPS.current = {
        load: function () {
            var xhr = new XMLHttpRequest();

            xhr.open('GET', '/get/reps/init');
            xhr.addEventListener('load', REPS.current.display);
            xhr.send();
        },
        display: function () {
            var data = JSON.parse(this.responseText),
                node = document.getElementById('reps'),
                dat = null,
                str = '',
                x = 0;

            for (x = 0; x < data.constructor.length; x += 1) {
                str = data.firstname + ' ' + data.lastname;
                dat = CREATE.li(null, data.id, str);
                node.addEventListener('click', REPS.info.load);
                node.appendChild(dat);
            }
        },
    };

    REPS.info = {
        load: function () {
            var xhr = new XMLHttpRequest();

            xhr.open('GET', '/get/reps/info');
            xhr.addEventListener('load', REPS.info.display);
            xhr.send();
        },
        display: function () {
            var data = JSON.parse(this.responseText),
                cellphone = document.getElementsByClassName('cellphone'),
                faxnumber = document.getElementsByClassName('faxnumber'),
                workphone = document.getElementsByClassName('workphone'),
                tmp = null,
                x = 0;

            document.getElementById('firstname').value = data.firstname;
            document.getElementById('lastname').value = data.lastname;
            document.getElementById('title').value = data.position;
            document.getElementById('emailaddress').value = data.email;
            document.getElementById('phonenumber').value = data.phone;
            document.getElementById('biography').value = data.biography;
            if (cellphone) {
                cellphone[0].value = data.cellphone;
            }
            if (faxnumber) {
                faxnumber[0].value = data.faxnumber;
            }
            if (workphone) {
                workphone[0].value = data.workphone;
            }

            REPS.editId = data.id;
        },
        addphone: function () {
            var selected = document.getElementById('addphone').selectedIndex,
                holder = document.getElementById('phoneholder');

            switch (selected) {
                case 0:
                    CREATE.input('text', 'Work Phone', 'workphone', holder);
                break;

                case 1:
                    CREATE.input('text', 'Cell Phone', 'cellphone', holder);
                break;

                case 2:
                    CREATE.input('text', 'Fax Number', 'faxnumber', holder);
                break;

                case 3:
                    CREATE.input('text', 'Phone', 'phonenumber', holder);
                break;

                default:
                break;
            }
        },
        modify: {
            post: function () {
                var xhr = new XMLHttpRequest(),
                    formData = new FormData(),
                    phone = document.getElementsByClassName('phonenumber'),
                    workphone = document.getElementsByClassName('workphone'),
                    cellphone = document.getElementsByClassName('cellphone'),
                    faxnumber = document.getElementsByClassName('workphone'),
                    tmp = '',
                    x = 0;

                formData.append('id', REPS.editId);
                formData.append('firstname', document.getElementById('firstname').value);
                formData.append('lastname', document.getElementById('lastname').value);
                formData.append('title', document.getElementById('title').value);
                formData.append('email', document.getElementById('emailaddress').value);
                if (phone) {
                    for (x = 0; x < phone.length; x += 1)
                        tmp += phone[x].value + ',';
                    if (tmp.length > 0)
                        formData.append('phone', tmp);
                    tmp = '';
                }
                if (workphone) {
                    for (x = 0; x < workphone.length; x += 1)
                        tmp += workphone[x].value + ',';
                    if (tmp.length > 0)
                        formData.append('workphone', tmp);
                    tmp = '';
                }
                if (cellphone) {
                    for (x = 0; x < cellphone.length; x += 1)
                        tmp += cellphone[x].value + ',';
                    if (tmp.length > 0)
                        formData.append('cellphone', tmp);
                    tmp = '';
                }
                if (faxnumber) {
                    for (x = 0; x < faxnumber.length; x += 1)
                        tmp += faxnumber[x].value + ',';
                    if (tmp.length > 0)
                        formData.append('faxnumber', tmp);
                    tmp = '';
                }

                xhr.open('POST', '/post/reps/info');
                xhr.addEventListener('load', REPS.info.modify.parse);
                xhr.send(formData);
            },
            parse: function () {
                console.log(JSON.parse(this.responseText));
            }
        }
    };

    REPS.current.load();
    document.getElementById('btnaddphone').addEventListener('click', REPS.info.addphone);
    document.getElementById('modify').addEventListener('click', REPS.info.modify.post);
})(console, document, window);
