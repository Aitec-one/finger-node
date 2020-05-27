const finger = require('./finger');
const fs = require('fs');

const tmplFile = "templates.txt";

fs.readFile(tmplFile, "utf8", function(err, data) {
    if (err) {
        console.log("error: " + err);
        return
    }

    if (data === "") {
        console.log("Enroll finger");
        finger.enroll((err, result) => {
            if (err) {
                console.log("Error: " + err)
            } else {
                console.log("enrolled:");
                console.log(result);
                fs.writeFile(tmplFile, result, function(err) {
                    if(err) {
                        return console.log(err);
                    }
                    console.log("Template was stored");
                });
            }
        });

    } else {
        console.log("verifing finger");
        console.log(data);
        finger.verify(data, (err, result) => {
            console.log(`err: ${err}, result: ${result}`);
            if (result === "verified") {
                console.log("verified");
            } else {
                console.log("not verified");
            }
        });
    }
});

