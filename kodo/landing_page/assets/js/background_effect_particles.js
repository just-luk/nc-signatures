
var particleNumber = 75;

window.requestAnimFrame = (function(){
    return window.requestAnimationFrame ||
        window.webkitRequestAnimationFrame ||
        window.mozRequestAnimationFrame ||
        function(callback){
            window.setTimeout(callback, 1000 / 25);
        };
})();

const element = document.querySelector('#header')
const style = getComputedStyle(element)
var [r, g, b] = style.backgroundColor.slice(4, -1).split(",").map(Number);
var colorAdd = 50;

container = document.getElementById('background_effect');
var c = document.createElement("canvas");
container.appendChild(c);

var ctx = c.getContext('2d');


$(window).resize(function() {
  c.width = document.body.clientWidth;
  c.height = window.innerHeight;
});
$(window).resize();

var particles = [];
for(i = 0; i < particleNumber; i++) {
  setTimeout(function(){
    particles.push(new createParticle(
      `rgba(${r+colorAdd * Math.random()}, ${g+colorAdd * Math.random()}, ${b+colorAdd * Math.random()}, 0.5)`));
  }, i * 15);
}

function createParticle(color) {

  this.color = color

  this.reset = function() {
    this.x = Math.random() * c.width;
    this.y = Math.random() * c.height;
    this.vx = (Math.random() - 0.5) * 8;
    this.vy = (Math.random() - 0.5) * 8;
    this.life = Math.random() * 40;
  }
  this.reset();
}

function draw() {
  requestAnimFrame(draw);

  ctx.clearRect(0, 0, c.width, c.height);
  particles.forEach(function(p)
  {
    ctx.beginPath();
    ctx.fillStyle = p.color;
    ctx.fillRect(p.x, p.y, p.life, p.life);

    p.x+=p.vx;
    p.y+=p.vy;

    p.life*=0.9;

    if (p.life < 1) {
      p.reset();
    }
  });
}

draw();
