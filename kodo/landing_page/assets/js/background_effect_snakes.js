/*
Copyright (c) 2015 by Captain Anonymous (http://codepen.io/anon/pen/MavGrq)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

A Pen created at CodePen.io. You can find this one at http://codepen.io/anon/pen/MavGrq.

a rainbowed-down version of [grayscale triangle snakes](http://codepen.io/MateiGCopot/pen/eNEpdY), couldn't resist making it ;D

Forked from [Matei Copot](http://codepen.io/towc/)'s Pen [rainbow triangle snakes](http://codepen.io/towc/pen/KpvdNY/).
*/


jQuery(function($) {

    $.fn.createSnake = function(user_options) {
        var It = this;
        var w = undefined;
        var h = undefined;
        var snakes = [];
        var ctx = $(It)[0].getContext( '2d' );

        var options = {

            count: 50,
            variation: .3,
            baseLife: 50,
            addedLife: 20,

            repaintAlpha: 15,

            fps: 100,

            sizeGain: .5,

            red: 255,
            green: 255,
            blue: 255,

            positionHorizontal: 'center',
            positionVertical: 'center'
        };

        options = $.extend(true, options, user_options);

        $(window).resize(function() {
            w = $(It)[0].width = $(It).parent().width();
            h = $(It)[0].height = $(It).parent().height();

            if (options.positionHorizontal == "left") { options.cx = 0; }
            if (options.positionHorizontal == "center") { options.cx = w / 2; }
            if (options.positionHorizontal == "right") { options.cx = w - 1 ; }
            if (options.positionVertical == "top") { options.cy = 0; }
            if (options.positionVertical == "center") { options.cy = h / 2; }
            if (options.positionVertical == "bottom") { options.cy = h - 1; }
        });
        $(window).resize();

        function init() {
            while ( snakes.length < options.count )
            {
                snakes.push( new Snake );
            }
            setInterval(function() {
                update();
                render();
            }, 1000 / options.fps);
        }

        function update() {

            snakes.map( function( snake ) { snake.update(); } );
        }
        function render() {

            var screenData = ctx.getImageData(0, 0, w, h);
            for (let i = 3; i < screenData.data.length; i+=4)
            {
                screenData.data[i] -= options.repaintAlpha;
            }
            ctx.putImageData(screenData, 0, 0);
            snakes.map( function( snake ) { snake.render(); } );
        }

        function Snake() {

            this.reset();
        }

        Snake.prototype.reset = function() {

            this.x1 = options.cx + Math.random();
            this.x2 = options.cx + Math.random();
            this.x3 = options.cx + Math.random();
            this.y1 = options.cy + Math.random();
            this.y2 = options.cy + Math.random();
            this.y3 = options.cy + Math.random();

            this.rad = Math.random() * Math.PI * 2;

            this.direction = Math.random() < .5 ? 1 : -1;
            this.size = 1;
            this.life = options.baseLife + Math.random() * options.addedLife;

            this.color = 'rgb(red, green, blue, alpha)'
                .replace('red', options.red)
                .replace('green', options.green)
                .replace('blue', options.blue)
                .replace('alpha', 0.2 + Math.random() * 0.8);
        }

        Snake.prototype.update = function() {

            this.life --;

            this.size += 1 + (Math.random() * options.sizeGain);
            this.direction *= -1;
            this.rad += Math.random() * options.variation * (Math.random() < .5 ? 1 : -1) + Math.PI / 2 * this.direction;

            var x4 = this.x3 + Math.cos( this.rad ) * this.size;
            var y4 = this.y3 + Math.sin( this.rad ) * this.size;

            this.x1 = this.x2;
            this.y1 = this.y2;
            this.x2 = this.x3;
            this.y2 = this.y3;
            this.x3 = x4;
            this.y3 = y4;

            if ( this.life <= 0|| this.x1 > w || this.x1 < 0 || this.y1 > h || this.y1 < 0 )
            {
                this.reset();
            }
        }
        Snake.prototype.render = function() {

            ctx.fillStyle = this.color;
            ctx.beginPath();
            ctx.moveTo( this.x1, this.y1 );
            ctx.lineTo( this.x2, this.y2 );
            ctx.lineTo( this.x3, this.y3 );
            ctx.fill();
        }

        init();
    }
});

$(function() {
    const element = document.querySelector('#header')
    const style = getComputedStyle(element)
    var [r, g, b] = style.backgroundColor.slice(4, -1).split(",").map(Number);
    var colorAdd = 50;

    container = document.getElementById( 'background_effect' );
    var canvas = document.createElement("canvas");
    container.appendChild(canvas);

    $(canvas).createSnake({

        count: 150,
        variation: 1.3,
        baseLife: 50,
        addedLife: 50,

        red: (colorAdd + r) % 255,
        green: (colorAdd + g) % 255,
        blue: (colorAdd + b) % 255,

        repaintAlpha: 25.5,

        fps: 25,

        sizeGain: 1.5,

        positionHorizontal: 'left',
        positionVertical: 'bottom',
    });
});
