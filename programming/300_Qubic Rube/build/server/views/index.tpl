<!DOCTYPE html>
<html lang="en">
	<head>
		<title>Qubic Rube - SECCON 2017 Online CTF</title> -->
		<!-- <title>three.js webgl - geometry - cube</title> -->
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, user-scalable=no, minimum-scale=1.0, maximum-scale=1.0">
		<style>
			body {
				margin: 0px;
				background-color: #000000;
				overflow: hidden;
			}
		</style>
	</head>
	<body>

		<script src="/js/three.min.js"></script>

		<script>

			var camera, scene, renderer;
			var cube;

			init();
			animate();

			function init() {

				camera = new THREE.PerspectiveCamera( 70, window.innerWidth / window.innerHeight, 1, 1000 );
				camera.position.z = 400;

				scene = new THREE.Scene();

				var loader = new THREE.TextureLoader();
				var texture1 = new loader.load( "/images/{{path}}_R.png" );
				var texture2 = new loader.load( "/images/{{path}}_L.png" );
				var texture3 = new loader.load( "/images/{{path}}_U.png" );
				var texture4 = new loader.load( "/images/{{path}}_D.png" );
				var texture5 = new loader.load( "/images/{{path}}_F.png" );
				var texture6 = new loader.load( "/images/{{path}}_B.png" );
				var materials = [
					new THREE.MeshBasicMaterial( { map: texture1 } ),
					new THREE.MeshBasicMaterial( { map: texture2 } ),
					new THREE.MeshBasicMaterial( { map: texture3 } ),
					new THREE.MeshBasicMaterial( { map: texture4 } ),
					new THREE.MeshBasicMaterial( { map: texture5 } ),
					new THREE.MeshBasicMaterial( { map: texture6 } ),
				];
				var faceMaterial = new THREE.MeshFaceMaterial( materials );
				var geometry = new THREE.BoxGeometry( 128, 128, 128 );
				cube = new THREE.Mesh( geometry, faceMaterial );
				scene.add( cube );

				renderer = new THREE.WebGLRenderer();
				renderer.setPixelRatio( window.devicePixelRatio );
				renderer.setSize( window.innerWidth, window.innerHeight );
				document.body.appendChild( renderer.domElement );


				window.addEventListener( 'resize', onWindowResize, false );

			}

			function onWindowResize() {

				camera.aspect = window.innerWidth / window.innerHeight;
				camera.updateProjectionMatrix();

				renderer.setSize( window.innerWidth, window.innerHeight );

			}

			function animate() {

				requestAnimationFrame( animate );

				cube.rotation.x += 0.005;
				cube.rotation.y += 0.01;

				renderer.render( scene, camera );

			}

		</script>

	</body>
</html>
