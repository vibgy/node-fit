'use strict';
const addon = require('../../build/Release/binding.node');
const assert = require('assert');
const request = require('request-promise');
const fs = require('fs');
var stream = require ('stream');
let Duplex = require('stream').Duplex;

var streamBuffers = require('stream-buffers');





const oauthHeader = 'oAuth oauth_consumer_key="54d00cdd-ee20-4dce-b111-4fcb39adbb37",oauth_token="d8f72b6d-07e2-45d2-bf4d-a6f55e10d5fc",oauth_signature_method="HMAC-SHA1",oauth_timestamp="1503953348",oauth_nonce="i0TIGJbWkfq",oauth_version="1.0",oauth_signature="O%2BK0K99gZ%2FnEJogerlLKXnM%2Fbjc%3D"';
//const oauthHeader = 'OAuth ';

const testFile = fs.createReadStream("ExampleActivityFile.fit");
//console.log(testFile);
//console.log(testFile);
var buffer =addon.encode("ExampleActivityFile.fit");
var binaryBuffer = new Buffer(buffer,'binary');

/*var bufferStream = new stream.PassThrough();
bufferStream.end()*/

/*var myReadableStreamBuffer = new streamBuffers.ReadableStreamBuffer();

  myReadableStreamBuffer.put(binaryBuffer);*/

  let readStream = new stream.PassThrough();
  readStream.end(binaryBuffer);

//console.log(buffer);
//console.log(typeof escape(file).toString('binary'))
//console.log( escape(file).toString('binary'))



fs.writeFile("./temp.fit", binaryBuffer, function(err) {
    if(err) {
        return console.log(err);
    }

    console.log("The file was saved!");
});

/*fs.writeFile("./temp.fit", buffer, function(err) {
    if(err) {
        return console.log(err);
    }

    console.log("The file was saved!");
});*/


/*function bufferToStream(buffer) {
  let stream = new Duplex();
  stream.push(buffer);
  stream.push(null);
  console.log(stream)
  return stream;
}

readStream.once("open", () => {

});*/

/*const options = {
    formData: {
        //file: new Buffer(buffer,'binary')
        //file: bufferToStream(binaryBuffer)
        //file: fs.createReadStream(binaryBuffer)
        //file: fs.createReadStream("temp.fit")
        //file: myReadableStreamBuffer
        file : readStream
      //},
      },
    headers: {
        "authorization": oauthHeader,
        "content-type": "multipart/form-data",
    },
    method: "POST",
    uri: "https://apis.garmin.com/activityupload/upload",
};//*/


/*request(options, function (error, response, body) {
    console.log("********************error");
    console.log(error);
    console.log("***********************response");
    console.log(response);
    console.log(error);
    console.log("*********************body");
    console.log(body);
})//*/

console.log();

console.log("Hooray!  The addon worked as expected.");