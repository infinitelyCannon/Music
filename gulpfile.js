var gulp = require('gulp');
var source = require('vinyl-source-stream');
var browserify = require('browserify');
//var babelify = require('babelify');
var rename = require('gulp-rename');
var uglify = require('gulp-uglify');
var sass = require('gulp-sass');
var pump = require('pump');

gulp.task('make-css', function(){
    gulp.src('./src/sass/root.scss')
    .pipe(sass({
        includePaths: ['lib'],
        omitSourceMapUrl: false,
        outFile: null,
        outputStyle: 'compressed'
    }).on('error', sass.logError))
    .pipe(rename('bundle.min.css'))
    .pipe(gulp.dest('./dist/css'));
});

gulp.task('make', function(){
    browserify('./src/index.js', {debug: false})
        .transform("babelify", {presets: ["babel-preset-env", "babel-preset-react"]})
        .exclude('electron')
        .exclude("fs")
        .exclude('url')
        .bundle()
        .pipe(source('./src/index.js'))
        .pipe(rename('main.js'))
        .pipe(gulp.dest('./dist'));
});

gulp.task('compress', function(){
    pump([
        gulp.src('./dist/main.js'),
        uglify(),
        gulp.dest('./dist')
    ]);
});

gulp.task('watch', function(){
    gulp.watch('./src/**/*.js', ['make']);
});

gulp.task('default', ['make', 'make-css']);