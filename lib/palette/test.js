console.log("Begining process. . .");

const Palette = require('./build/Debug/palette');
var data = require('./test-data');

var swatches = Palette.generateSync({
    image: data.infinity,
    width: 750,
    height: 750
});

console.log(swatches);