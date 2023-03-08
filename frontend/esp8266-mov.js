const fs = require("fs");
const path = require("path");

const buildPath = path.resolve(__dirname, "build");
const ESP8266DataPath = path.resolve(__dirname, "..", "data", "view");

const blackListFolders = ["static"];

const replaceImports = [];

function copyFolderRecursiveSync(origin, destination) {

    const files = fs.readdirSync(origin);

    for (const file of files) {

        const filePath = path.resolve(origin, file);
        const destinationFilePath = path.resolve(destination, file);

        const ls = fs.lstatSync(filePath);

        if (ls.isDirectory()) {

            if (!blackListFolders.includes(file)) continue;

            if (!fs.existsSync(destination)) {

                fs.mkdirSync(destination)
            }
            copyFolderRecursiveSync(filePath, destinationFilePath);

        } else {

            if (fs.existsSync(destinationFilePath)) {

                fs.unlinkSync(destinationFilePath)
            }

            if (file.search(".LICENSE") !== -1) {

                continue;
            }

            console.log(`[esp8266-mov] - Copying ${file}`);

            const isTheMainCssFile = new RegExp(/^main\.[0-9a-f]{8}\.css$/).test(file);
            const isTheMainJsFile = new RegExp(/^main\.[0-9a-f]{8}\.js$/).test(file);

            if (isTheMainCssFile) {

                fs.copyFileSync(filePath, path.resolve(destination, "main.css"));
                replaceImports.push({ name: file, replaceFor: "main.css" });
                continue;
            }

            if (isTheMainJsFile) {

                fs.copyFileSync(filePath, path.resolve(destination, "main.js"));
                replaceImports.push({ name: file, replaceFor: "main.js" });
                continue;
            }

            fs.copyFileSync(filePath, destinationFilePath);
        }
    }   
}

function updateImportsToESP8266() {

    const htmlPath = path.resolve(__dirname, "..", "data", "view", "index.html");
    const html = fs.readFileSync(htmlPath);

    let aux = html.toString();

    for (const file of replaceImports) {

        console.log(`[esp8266-mov] - Replacing ${file.name} for ${file.replaceFor}`);
        aux = aux.replace(new RegExp(file.name), file.replaceFor);
    }

    fs.writeFileSync(htmlPath, aux);

}

copyFolderRecursiveSync(buildPath, ESP8266DataPath);

// Move all static files to root dir
copyFolderRecursiveSync(path.resolve(buildPath, "static", "js"), ESP8266DataPath);
copyFolderRecursiveSync(path.resolve(buildPath, "static", "css"), ESP8266DataPath);

updateImportsToESP8266();