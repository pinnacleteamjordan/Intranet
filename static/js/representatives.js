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
            var xhr = XMLHttpRequest();

            xhr.open('GET', '/get/reps/info');
            xhr.addEventListener('load', REPS.info.display);
            xhr.send();
        },
        display: function () {
            var data = JSON.parse(this.responseText);

            console.log(data);
        }
    };

    REPS.current.load();
})(console, document, window);
