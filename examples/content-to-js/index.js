'use strict'
const FitParser = require("../../lib/fitparser.js").FitParser;
const activityObj = require("../../test/activity-from-data.js");
const fs = require("fs");

const fitParser = new FitParser();

const handleWritingFile = (err, binaryBuffer) => {
  if (err) {
    console.log("[Error]", err);
  }
};


var buffer = fitParser.encode(activityObj);
const binaryBuffer = new Buffer(buffer, "binary");
fs.writeFile("temp.fit", binaryBuffer, err => handleWritingFile(err, binaryBuffer));


