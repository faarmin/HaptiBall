const char html_page[] PROGMEM = R"RawString(
  
<!DOCTYPE HTML><html>

<head>
  <title>Testing Suite</title>>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/107/three.min.js"></script>
</head>
<body>
  <div class="topnav">
    <h1>Testing Suite</h1>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card">
        <p class="card-title">GYROSCOPE</p>
        <p><span class="reading">X: <span id="gyroX"></span> rad</span></p>
        <p><span class="reading">Y: <span id="gyroY"></span> rad</span></p>
        <p><span class="reading">Z: <span id="gyroZ"></span> rad</span></p>
      </div>
      <div class="card">
        <p class="card-title">ACCELEROMETER</p>
        <p><span class="reading">X: <span id="accX"></span> ms<sup>2</sup></span></p>
        <p><span class="reading">Y: <span id="accY"></span> ms<sup>2</sup></span></p>
        <p><span class="reading">Z: <span id="accZ"></span> ms<sup>2</sup></span></p>
      </div>
      <div class="card">
        <p class="card-title">ANGULAR ROTATION</p>
        <p>
            <span class="reading">X: <span id="angX"></span> grados </span>
            <span class="reading">-><span id="angXrad"></span> radianes</span>
        </p>
        <p>
            <span class="reading">Y: <span id="angY"></span> grados </sup></span>
            <span class="reading">-><span id="angYrad"></span> radianes</sup></span>
        </p>

      </div>
      <div class="container">
        <button id="Sup" onclick="accionarMotor(Sup)">motorSup</button>
        <div class="base">
              <button class="base-btn" id="InfIz" onclick="accionarMotor(InfIz)">motorInfIz</button>
              <button class="base-btn" id="InfDer" onclick="accionarMotor(InfDer)">motorInfDer</button>
        </div>
      </div>

      <div class="card">
      <br><br>
        <button id="reset" onclick="resetPosition(this)">RESET POSITION</button>
      </div>
    </div>
    <div class="cube-content">
      <div id="3Dcube"></div>
    </div>
  </div>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/107/three.min.js"></script>
<script>

                                    let scene, camera, rendered, cube;

                                    function parentWidth(elem) {
                                      return elem.parentElement.clientWidth;
                                    }

                                    function parentHeight(elem) {
                                      return elem.parentElement.clientHeight;
                                    }

                                    function init3D(){
                                      scene = new THREE.Scene();
                                      scene.background = new THREE.Color(0xffffff);

                                      camera = new THREE.PerspectiveCamera(75, parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube")), 0.1, 1000);

                                      renderer = new THREE.WebGLRenderer({ antialias: true });
                                      renderer.setSize(parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")));

                                      document.getElementById('3Dcube').appendChild(renderer.domElement);

                                      // Create a geometry
                                      const geometry = new THREE.BoxGeometry(5,1, 4);

                                      // Materials of each face
                                      var cubeMaterials = [
                                        new THREE.MeshBasicMaterial({color:0x03045e}),
                                        new THREE.MeshBasicMaterial({color:0x023e8a}),
                                        new THREE.MeshBasicMaterial({color:0x0077b6}),
                                        new THREE.MeshBasicMaterial({color:0x03045e}),
                                        new THREE.MeshBasicMaterial({color:0x023e8a}),
                                        new THREE.MeshBasicMaterial({color:0x0077b6}),
                                      ];

                                      const material = new THREE.MeshFaceMaterial(cubeMaterials);

                                      cube = new THREE.Mesh(geometry, material);
                                      scene.add(cube);
                                      camera.position.z = 5;
                                      renderer.render(scene, camera);
                                    }

                                    // Resize the 3D object when the browser window changes size
                                    function onWindowResize(){
                                      camera.aspect = parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube"));
                                      //camera.aspect = window.innerWidth /  window.innerHeight;
                                      camera.updateProjectionMatrix();
                                      //renderer.setSize(window.innerWidth, window.innerHeight);
                                      renderer.setSize(parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")));

                                    }

                                    window.addEventListener('resize', onWindowResize, false);

                                    // Create the 3D representation
                                    init3D();

                                    // Create events for the sensor readings
                                    if (!!window.EventSource) {
                                      var source = new EventSource('/events');

                                      source.addEventListener('open', function(e) {
                                        console.log("Events Connected");
                                      }, false);

                                      source.addEventListener('error', function(e) {
                                        if (e.target.readyState != EventSource.OPEN) {
                                          console.log("Events Disconnected");
                                        }
                                      }, false);

                                      source.addEventListener('gyro_readings', function(e) {
                                        //console.log("gyro_readings", e.data);
                                        var obj = JSON.parse(e.data);
                                        document.getElementById("gyroX").innerHTML = obj.gyroX;
                                        document.getElementById("gyroY").innerHTML = obj.gyroY;
                                        document.getElementById("gyroZ").innerHTML = obj.gyroZ;

                                        // Change cube rotation after receiving the readinds
                                        cube.rotation.x = obj.gyroY;
                                        cube.rotation.z = obj.gyroX;
                                        cube.rotation.y = obj.gyroZ;
                                        renderer.render(scene, camera);
                                      }, false);

                                      source.addEventListener('accelerometer_readings', function(e) {
                                        console.log("accelerometer_readings", e.data);
                                        var obj = JSON.parse(e.data);
                                        document.getElementById("accX").innerHTML = obj.accX;
                                        document.getElementById("accY").innerHTML = obj.accY;
                                        document.getElementById("accZ").innerHTML = obj.accZ;
                                      }, false);
                                    
                                    source.addEventListener('combined_reading', function(e) {
                                        console.log("combined_reading", e.data);
                                        var obj = JSON.parse(e.data);
                                        document.getElementById("angX").innerHTML = obj.angX;
                                        document.getElementById("angY").innerHTML = obj.angY;
                                        document.getElementById("angXrad").innerHTML = obj.angXrad;
                                        document.getElementById("angYrad").innerHTML = obj.angYrad;

                                      }, false);
                                    }

                                    function resetPosition(element){
                                      var xhr = new XMLHttpRequest();
                                      xhr.open("GET", "/"+element.id, true);
                                      console.log(element.id);
                                      xhr.send();
                                    }
                                    function accionarMotor(element){
                                      var xhr = new XMLHttpRequest();
                                      xhr.open("GET", "/"+element.id, true);
                                      console.log(element.id);
                                      xhr.send();
                                    }                                    


</script>
</body>
</html>


)RawString";