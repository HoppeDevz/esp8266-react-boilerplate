const fs = require("fs");
const path = require("path");

const buildPath = path.resolve(__dirname, "build");
const ESP8266DataPath = path.resolve(__dirname, "..", "data", "view");

const blackListFolders = ["static"];

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

            fs.copyFileSync(filePath, destinationFilePath);
        }
    }   
}

copyFolderRecursiveSync(buildPath, ESP8266DataPath);

// Move all static files to root dir
copyFolderRecursiveSync(path.resolve(buildPath, "static", "js"), ESP8266DataPath)
copyFolderRecursiveSync(path.resolve(buildPath, "static", "css"), ESP8266DataPath)