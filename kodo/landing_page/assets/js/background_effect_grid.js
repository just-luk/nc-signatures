
$(function() {
  const element = document.querySelector('#header')
  const style = getComputedStyle(element)
  var [r, g, b] = style.backgroundColor.slice(4, -1).split(",").map(Number);
  var colorAdd = 25;

  r = (colorAdd + r) % 255;
  g = (colorAdd + g) % 255;
  b = (colorAdd + b) % 255;

  const win = typeof window == 'object'
  const points_option = 30;
  const maxDistance = 20;
  const spacing = 15;
  const color = new THREE.Color(`rgb(${r}, ${g}, ${b})`);
  var container;
  var camera, scene, renderer;
  var width, height;
  var points = [];
  var linePositions, linesMesh;
  var t = 0;


  init();
  animate();

  function init() {

    container = document.getElementById( 'background_effect' );
    renderer = new THREE.WebGLRenderer({
      alpha: true,
      antialias: true
    });

    window.addEventListener( 'resize', onWindowResize, false );
    onWindowResize();
    container.appendChild( renderer.domElement );

    scene = new THREE.Scene();


    var group = new THREE.Group()
    group.position.set(-50,-20,0)
    scene.add(group)
    const n = points_option;
    const numPoints = n * n * 2;
    linePositions = new Float32Array( numPoints * numPoints * 3 );

    const geometry = new THREE.BufferGeometry();
    geometry.addAttribute('position', new THREE.BufferAttribute(linePositions, 3).setDynamic(true));

    const material = new THREE.LineBasicMaterial({
      color: color,
      blending: null,
      transparent: true,
      linewidth: 2
    });

    linesMesh = new THREE.LineSegments( geometry, material )
    group.add( linesMesh )
    for (let i = 0; i<=n; i++) {
      for (let j = 0; j<=n; j++) {
        const y = 0
        const x = ((i - (n/2)) * spacing)
        let z = ((j - (n/2)) * spacing)

        let sphere = new THREE.Object3D()
        group.add( sphere )
        sphere.ox = x
        sphere.oy = y
        sphere.oz = z
        sphere.position.set(x,y,z)
        sphere.r = 0
        points.push(sphere)
      }
    }

    camera = new THREE.PerspectiveCamera(20, width / height, 0.01, 10000);
    camera.position.set(50, 100, 150)
  }



  function onWindowResize() {
    width = document.body.clientWidth;
    height = window.innerHeight;

    var ref, ref1
    if ((ref = camera) != null) {
      ref.aspect = width / height
    }
    if ((ref1 = camera) != null) {
      if (typeof ref1.updateProjectionMatrix === "function") {
        ref1.updateProjectionMatrix()
      }
    }
    if (renderer) {
      renderer.setSize(width, height);
      renderer.setPixelRatio(window.devicePixelRatio / 1)
    }
  }

  function animate() {

    // Step time
    t += 1

    const c = camera
    if (Math.abs(c.tx - c.position.x) > 0.01) {
      var diff = c.tx - c.position.x
      c.position.x += diff * 0.02
    }
    if (Math.abs(c.ty - c.position.y) > 0.01) {
      var diff = c.ty - c.position.y
      c.position.y += diff * 0.02
    }
    if (win && window.innerWidth < 480) {
      c.lookAt( new THREE.Vector3( -10, 0, 0 ) )
    } else if (win && window.innerWidth < 720) {
      c.lookAt( new THREE.Vector3( -20, 0, 0 ) )
    } else c.lookAt( new THREE.Vector3( -40, 0, 0 ) )

    let vertexpos = 0
    let colorpos = 0
    let numConnected = 0

    for (let i = 0; i < points.length; i++)
    {

      const p = points[i]

      p.scale.x = p.scale.y = p.scale.z

      // Waves
      p.position.y = 2 * Math.sin(p.position.x/10 + t*0.01 + p.position.z/10 * 0.5)

      for (let j = i; j < points.length; j++) {
        const p2 = points[j]
        const dx = p.position.x - p2.position.x
        const dy = p.position.y - p2.position.y
        const dz = p.position.z - p2.position.z
        var dist = Math.sqrt( (dx * dx) + (dy * dy) + (dz * dz) )
        if (dist < maxDistance) {
          linePositions[ vertexpos++ ] = p.position.x
          linePositions[ vertexpos++ ] = p.position.y
          linePositions[ vertexpos++ ] = p.position.z
          linePositions[ vertexpos++ ] = p2.position.x
          linePositions[ vertexpos++ ] = p2.position.y
          linePositions[ vertexpos++ ] = p2.position.z

          numConnected++
        }
      }
    }
    linesMesh.geometry.setDrawRange( 0, numConnected * 2 )
    linesMesh.geometry.attributes.position.needsUpdate = true

    renderer.render(scene, camera)

    requestAnimationFrame( animate );
  }
});
