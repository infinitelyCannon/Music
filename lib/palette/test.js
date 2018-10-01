console.log("Begining process. . .");

const Palette = require('./build/Debug/palette');
var data = require('./test-data');

var swatches = Palette.generateSync({
    image: data.faultLine,
    width: 150,
    height: 150
});

console.log(swatches);