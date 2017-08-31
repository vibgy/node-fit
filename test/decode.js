require("mocha");
var should = require('should');
var sinon = require('sinon');
var path = require('path');
var FitParser = require('../lib/fitparser').FitParser;

describe("decode()", function() {
   describe("#Exceptions", function() {

      it("should throw an arguments error when input is undefined", function() {
         var fitParser = new FitParser();
         fitParser.on('error', function(e) {
            e.message.should.equal('Wrong number of arguments');
         });
         fitParser.on('end', shouldNotHappen);
         fitParser.decode();
      });

      it("should throw an arguments error when input is not a string", function() {
         var fitParser = new FitParser();
         fitParser.on('error', function(e) {
            e.message.should.equal('Argument must be a string.');
         });
         fitParser.on('end', shouldNotHappen);
         fitParser.decode(1);
      });

      it("should throw an error when the file does not exist", function() {
         var fitParser = new FitParser();
         fitParser.on('error', function(e) {
            e.message.should.equal('Error opening file.');
         });
         fitParser.on('end', shouldNotHappen);
         fitParser.decode("fileNotFound");
      });

      it("should throw an error when the file integrity check fails", function() {
         var fitParser = new FitParser();
         fitParser.on('error', function(e) {
            e.message.should.equal('FIT file integrity failed.');
         });
         fitParser.on('end', shouldNotHappen);
         var fileName = path.resolve('test-files/poorIntegrity.fit');
         fitParser.decode(fileName);
      });

      it("should not throw an error if a good file", function() {
         var fitParser = new FitParser();
         fitParser.on('error', shouldNotHappen);
         var fileName = path.resolve('test-files/Activity.fit');
         fitParser.decode(fileName);
      });
   });

   describe("#Messages", function() {

      it("should emit messages", function() {
         var fitParser = new FitParser();
         var onMessage = sinon.spy();
         fitParser.on('message', function(msg) {
            msg.should.have.property('type');
            msg.should.have.property('value');
            msg.should.have.property('units');
            onMessage();
         });
         var fileName = path.resolve('test-files/Activity.fit');
         fitParser.decode(fileName);
         onMessage.callCount.should.be.above(0);
      });

      it("should emit record messages", function() {
         var fitParser = new FitParser();
         var onRecord = sinon.spy();
         fitParser.on('record', function(msg) {
            msg.should.have.property('timestamp');
            msg.should.have.property('position_lat');
            msg.should.have.property('position_long');
            msg.should.have.property('altitude');
            msg.should.have.property('distance');
            msg.should.have.property('speed');
            msg.should.not.have.property('heart_rate');
            onRecord();
         });
         var fileName = path.resolve('test-files/Activity.fit');
         fitParser.decode(fileName);
         //onRecord.callCount.should.equal(14);
      });

      it("record timestamp should be a DateTime", function() {
         var fitParser = new FitParser();
         var onRecord = sinon.spy();
         fitParser.on('record', function(msg) {
            msg.timestamp.should.be.an.instanceof(Date);
            msg.timestamp.should.be.within(new Date('Mon Apr 09 2012 16:22:26 GMT-0500'), new Date('Mon Apr 09 2012 16:22:39 GMT-0500'))
         });
         var fileName = path.resolve('test-files/Activity.fit');
         fitParser.decode(fileName);
         //onRecord.callCount.should.equal(14);
      });

      it("record position_lat should be a number", function() {
         var fitParser = new FitParser();
         fitParser.on('record', function(msg) {
            msg.position_lat.should.be.within(495280402, 495280588);
         });
         var fileName = path.resolve('test-files/Activity.fit');
         fitParser.decode(fileName);
      });

      it("record position_long should be a number", function() {
         var fitParser = new FitParser();
         fitParser.on('record', function(msg) {
            msg.position_long.should.be.within(-872697257, -872696680);
         });
         var fileName = path.resolve('test-files/Activity.fit');
         fitParser.decode(fileName);
      });

      it("record distance should be a float", function() {
         var fitParser = new FitParser();
         fitParser.on('record', function(msg) {
            msg.distance.should.be.within(0.01, 5.74);
         });
         var fileName = path.resolve('test-files/Activity.fit');
         fitParser.decode(fileName);
      });

      it("record speed should be a float", function() {
         var fitParser = new FitParser();
         fitParser.on('record', function(msg) {
            msg.speed.should.be.within(0, 0.369);
         });
         var fileName = path.resolve('test-files/Activity.fit');
         fitParser.decode(fileName);
      });
   });

   describe("#Encode", function() {

      it.only("should encode a .fit file", function() {
         var fitParser = new FitParser();
         var onMessage = sinon.spy();
         var obj = require('./activity-from-data.js'); //{someVal: "xyz"};

         //console.log("print val *** " + fitParser.encode());
         console.log("print val *** " + fitParser.decode());

         //onMessage();

         fitParser.encode(obj, function(err, buffer) {
            console.log(buffer);
            //fs.writeFileSync("output.fit", buffer, 'binary');
            onMessage();
         });
         console.log(obj.timestamp);
         fitParser.on('done', function(buf) {
            console.log(buf);
            onMessage();
         });
         fitParser.encode(obj);
         onMessage.callCount.should.be.above(0);
      });
   });
});

function shouldNotHappen() {
   false.should.be.ok;
}

function shouldHappen() {
   true.should.be.ok;
}