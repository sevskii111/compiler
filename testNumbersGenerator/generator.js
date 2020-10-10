var RandExp = require("randexp");
var fs = require("fs");

let result = "";

let generator = new RandExp(
  /^[-+]?[0-9]{0,2}\.?[0-9]{1,2}([eE][-+]?[0-9]{1})?$/,
  1
);
for (let i = 0; i < 100; i++) {
  result += generator.gen() + "\n";
}

fs.writeFileSync("randomNumbers.txt", result);
