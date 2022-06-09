/*

ThreeJs custom waves
Original script by ThreeJS : https://threejs.org/examples/canvas_particles_waves.html
Modified version for Cloudoru by Kevin Rajaram : http://kevinrajaram.com
Date: 08/14/2014


*/

var SEPARATION = 150, AMOUNTX = 35, AMOUNTY = 15;

var container;
var camera, scene, renderer;
var particles, particle, count = 0;

init();
animate();

function init() {

	container = document.getElementById( 'background_effect' );

	camera = new THREE.PerspectiveCamera( 120, window.innerWidth / window.innerHeight, 100, 3000 );
	camera.position.y = 300; //changes how far back you can see i.e the particles towards horizon
	camera.position.z = 00; //This is how close or far the particles are seen

	camera.rotation.x = 0.35;
	// camera.rotation.y = 0.5;
	// camera.rotation.z = 0.5;

	scene = new THREE.Scene();

	particles = new Array();

	const geometry = new THREE.SphereGeometry( 0.3, 6, 6)
	const material =new THREE.LineBasicMaterial({color: 0xffffff});
	var i = 0;
	for ( var ix = 0; ix < AMOUNTX; ix ++ ) {

		for ( var iy = 0; iy < AMOUNTY; iy ++ ) {
			particle = particles[ i ++ ] = new THREE.Mesh( geometry, material )
			particle.position.x = ix * SEPARATION - ( ( AMOUNTX * SEPARATION ) / 2 );
			particle.position.z = iy * SEPARATION - ( ( AMOUNTY * SEPARATION ) - 10 );
			scene.add( particle );
		}
	}

	renderer = new THREE.WebGLRenderer({ alpha: true, antialias: true });
	renderer.setSize(
		document.body.clientWidth,
		window.innerHeight );
	container.appendChild( renderer.domElement );
	window.addEventListener( 'resize', onWindowResize, false );

}

function onWindowResize() {

	camera.aspect = window.innerWidth / window.innerHeight;
	camera.updateProjectionMatrix();

	renderer.setSize( document.body.clientWidth, window.innerHeight );

}

function animate() {

	requestAnimationFrame( animate );

	render();

}

function render() {

	var i = 0;

	for ( var ix = 0; ix < AMOUNTX; ix ++ ) {

		for ( var iy = 0; iy < AMOUNTY; iy ++ ) {

			particle = particles[ i++ ];
			particle.position.y = ( Math.sin( ( ix + count ) * 0.5 ) * 15 ) + ( Math.sin( ( iy + count ) * 0.5 ) * 15 );
			particle.scale.x = particle.scale.y = ( Math.sin( ( ix + count ) * 0.5 ) + 2 ) * 4 + ( Math.sin( ( iy + count ) * 0.5 ) + 1 ) * 4;

		}

	}

	renderer.render( scene, camera );

	// This increases or decreases speed
	count += 0.05;

}
