
var gulp = require('gulp');
var browserify = require('browserify');
var reactify = require('reactify');
var source = require('vinyl-source-stream');
var watchify = require('watchify');

function bundle(watch)
{
    var bundler;
    var input = './client/index.js';
    if (watch) {
        bundler = watchify(browserify(input, watchify.args));
    } else {
        bundler = browserify(input);
    }
    bundler.transform('reactify');
    
    function bundleProper()
    {
        var bundle = bundler.bundle();
        bundle
            .pipe(source('bundle.js'))
            .pipe(gulp.dest('./client'));
    }

    bundleProper();

    if (watch) {
        bundler.on('update', bundleProper);        
    }
}

gulp.task('browserify', function() {
    bundle(false);
});

gulp.task('watchify', function() {
    bundle(true);
});
