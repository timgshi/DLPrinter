
// These two lines are required to initialize Express in Cloud Code.
var express = require('express');
var app = express();

// Global app configuration section
app.set('views', 'cloud/views');  // Specify the folder to find templates
app.set('view engine', 'ejs');    // Set the template engine
app.use(express.bodyParser());    // Middleware for reading request body

// This is an example of hooking up a request handler with a specific request
// path and HTTP verb using the Express routing API.
app.get('/hello', function(req, res) {
  res.render('hello', { message: 'Congrats, you just set up your app!' });
});

app.get('/timdistance', function(req, res) {
	var diana_home = new Parse.GeoPoint({latitude: 33.99890, longitude: -118.45945});
	var DLLocation = Parse.Object.extend("DLLocation");
	var query = new Parse.Query(DLLocation);
	query.first().then(function(object) {
		var tim_location = object.get("location");
		var distance = Math.floor(diana_home.milesTo(tim_location));
		return res.send(distance.toString());
	}, function(error) {
		return res.send(500, error);
	});
});

// Attach the Express app to Cloud Code.
app.listen();
