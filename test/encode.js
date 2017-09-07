require("mocha");
var should = require('should');
var sinon = require('sinon');
var path = require('path');
var FitParser = require('../lib/fitparser').FitParser;

describe("encode()", function() {

   describe("#Encode", function() {

      it.only("should encode and return the content of a .fit file", function() {
         var fitParser = new FitParser();
         var onMessage = sinon.spy();
         var obj = require('./activity-from-data.js');
         var buffer= fitParser.encode(obj);
         console.log("buffer.length: "+buffer.length)
         buffer.should.be.a.String;
         buffer.should.not.be.empty;
      });
   });
});

function shouldNotHappen() {
   false.should.be.ok;
}

function shouldHappen() {
   true.should.be.ok;
}