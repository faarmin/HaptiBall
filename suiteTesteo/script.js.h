R"=====(

//Variables 
let scene, camera, rendered, sphere, hitArrowHelper,arrowHelpersGroup,pointsGroup,slider,cylinderHelpersGroup;
let numPower1,numPower2,numPower3,numPower4,numPower5,numPower6;
let numHitX,numHitY,numHitZ;
let fibonacciAct,regAct,loopCheck;
const vectors = [];
const directions = [];


//***********************************************************************************************************************************
//***********************************************************************************************************************************
//**********************************************ESCENA******************************************************

//Dimensionado
function parentWidth(elem) {
    return elem.parentElement.clientWidth;
}

function parentHeight(elem) {
    return elem.parentElement.clientHeight;
}

//Creacion de escena e inicio de threeJS
function createSceneJS(){
    console.log('Entra a createSceneJS');

    scene = new THREE.Scene();
    scene.background = new THREE.Color(0xffffff);

    camera = new THREE.PerspectiveCamera(75, parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube")), 0.1, 1000);

    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")));

    document.getElementById('3Dcube').appendChild(renderer.domElement);

    // Crear la geometría
    const sphereGeometry = new THREE.SphereGeometry(2.5, 32, 32);
    const sphereMaterial = new THREE.MeshBasicMaterial({
      color: 0x03045e,
      wireframe: true,
      opacity: 0.1,
      transparent: false,
    });

    //Añadimos esfera
    sphere = new THREE.Mesh(sphereGeometry, sphereMaterial);
    scene.add(sphere);

    // Grupo de puntos
    pointsGroup = new THREE.Group();
    scene.add(pointsGroup);

    // Grupo de arrowHelpers
    arrowHelpersGroup = new THREE.Group();
    scene.add(arrowHelpersGroup);

    sphere.add(pointsGroup);
    sphere.add(arrowHelpersGroup);

    //nueva distribucion de vectores
    cylinderHelpersGroup = new THREE.Group();
    sphere.add(cylinderHelpersGroup);

    camera.position.z = 5;
    renderer.render(scene, camera);
    console.log(sphere.radius);
    console.log('Sale createSceneJS');
}

// Ajuste de tamaño responsive
function onWindowResize(){
    console.log('WindowResize');
    camera.aspect = parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube"));
    camera.updateProjectionMatrix();
    renderer.setSize(parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")));
    
}

//Reset de la esfera (posicion inicial vertical)
function resetPosition(element){
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/"+element.id, true);
    console.log(element.id);
    xhr.send();
}


/************************************************************************************************************************
*************************************************************************************************************************
************************************************-EN MARCHA-*********************************************************************/

window.addEventListener('resize', onWindowResize, false);

// Create the 3D representation
createSceneJS();
fibonacciAct= true;
distributePointsOnSphere(6);
createVectors();

//***********************DOCUMENT GETTERS********************
slider1 = document.getElementById("slider1");
slider2 = document.getElementById("slider2");
slider3 = document.getElementById("slider3");
slider4 = document.getElementById("slider4");
slider5 = document.getElementById("slider5");
slider6 = document.getElementById("slider6");
numPower1 = document.getElementById("numPower1");
numPower2 = document.getElementById("numPower2");
numPower3 = document.getElementById("numPower3");
numPower4 = document.getElementById("numPower4");
numPower5 = document.getElementById("numPower5");
numPower6 = document.getElementById("numPower6");

slider1.oninput = function () {
  const n = parseInt(this.value);
  numPower1.textContent = n;
};
slider2.oninput = function () {
  const n = parseInt(this.value);
  numPower2.textContent = n;
};
slider3.oninput = function () {
  const n = parseInt(this.value);
  numPower3.textContent = n;
};
slider4.oninput = function () {
  const n = parseInt(this.value);
  numPower4.textContent = n;
};
slider5.oninput = function () {
  const n = parseInt(this.value);
  numPower5.textContent = n;
};
slider6.oninput = function () {
  const n = parseInt(this.value);
  numPower6.textContent = n;
};

hitX = document.getElementById("hitX");
hitY = document.getElementById("hitY");
hitZ = document.getElementById("hitZ");
numHitX = document.getElementById("numHitX");
numHitY = document.getElementById("numHitY");
numHitZ = document.getElementById("numHitZ");

hitX.oninput = function () {
  const n = parseInt(this.value);
  numHitX.textContent = n;
};

hitY.oninput = function () {
  const n = parseInt(this.value);
  numHitY.textContent = n;
};

hitZ.oninput = function () {
  const n = parseInt(this.value);
  numHitZ.textContent = n;
};

//*****************************LISTENERS*********************
document.getElementById('myCheckbox').addEventListener('change', function() {
    fibonacciAct = this.checked;
    arrowHelpersGroup.visible = fibonacciAct;
    pointsGroup.visible = fibonacciAct;
    console.log('Checkbox activado:', fibonacciAct);
});

document.getElementById('myCheckbox2').addEventListener('change', function() {
    regAct = this.checked;
    cylinderHelpersGroup.visible = regAct;
    pointsGroup.visible = regAct;
    console.log('Checkbox activado:', regAct);
});

document.getElementById('loopCheck').addEventListener('change', function() {
    loopCheck = this.checked;
    console.log('Checkbox activado:', loopCheck);
});

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

    // Change sphere rotation after receiving the readinds
    sphere.rotation.x = obj.gyroY;
    sphere.rotation.y = obj.gyroZ;
    sphere.rotation.z = obj.gyroX;
    renderer.render(scene, camera);
    }, false);

source.addEventListener('accelerometer_readings', function(e) {
    //console.log("accelerometer_readings", e.data);
    var obj = JSON.parse(e.data);
    document.getElementById("accX").innerHTML = obj.accX;
    document.getElementById("accY").innerHTML = obj.accY;
    document.getElementById("accZ").innerHTML = obj.accZ;
    }, false);

source.addEventListener('combined_reading', function(e) {
    //console.log("combined_reading", e.data);
    var obj = JSON.parse(e.data);
    document.getElementById("angX").innerHTML = obj.angX;
    document.getElementById("angY").innerHTML = obj.angY;
    document.getElementById("angXrad").innerHTML = obj.angXrad;
    document.getElementById("angYrad").innerHTML = obj.angYrad;

    }, false);
}


//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//********************************************************fUNCIONES ADICIONALES*******************************************************************


function distributePointsOnSphere(n) {
        pointsGroup.clear();
        arrowHelpersGroup.clear();
        const goldenAngle = (1 + 5 ** 0.5) / 2;
        for (let i = 0; i < n; i++) {
          const theta = (2 * Math.PI * i) / goldenAngle;
          const phi = Math.acos(1 - (2 * (i + 0.5)) / n);
          const x = Math.cos(theta) * Math.sin(phi);
          const y = Math.sin(theta) * Math.sin(phi);
          const z = Math.cos(phi);
          const pointGeometry = new THREE.SphereGeometry(0.010, 32, 32);
          const pointMaterial = new THREE.MeshBasicMaterial({
            color: 0xffffff,
            opacity: 0.25,
            transparent: true,
          });
          const point = new THREE.Mesh(pointGeometry, pointMaterial);
          point.position.set(x, y, z);
          pointsGroup.add(point);

          // Añadir flechas que apunten a cada punto
          const arrowHelper = new THREE.ArrowHelper(
            point.position.clone().normalize(),
            new THREE.Vector3(0, 0, 0),
            point.position.length()*2.5,
            0xff0000
          );
          arrowHelpersGroup.add(arrowHelper);
        }
}


//Crea vectores ortogonales
function createVectors() {
  const radius = 2.5; // Radius of the sphere
  const cylinderRadius = 0.025 ; // Radius of the cylinder
  const vectorLength = radius;

  // Create the 6 vectors
  const directions = [
      new THREE.Vector3(0, 1, 0), 
      new THREE.Vector3(0, -1, 0), 
      new THREE.Vector3(1, 0, 0),  
      new THREE.Vector3(-1, 0, 0),
      new THREE.Vector3(0, 0, 1),  
      new THREE.Vector3(0, 0, -1)  
  ];

  for (const direction of directions) {
      const cylinderGeometry = new THREE.CylinderGeometry(cylinderRadius, cylinderRadius, vectorLength, 16);
      const cylinderMaterial = new THREE.MeshBasicMaterial({ color: 0xff0000 });
      const cylinder = new THREE.Mesh(cylinderGeometry, cylinderMaterial);

      const midpoint = direction.clone().multiplyScalar(vectorLength / 2);
      cylinder.position.copy(midpoint);
      cylinder.lookAt(direction.clone().multiplyScalar(vectorLength));
      //Rotación
      cylinder.rotateX(Math.PI / 2); 

      cylinderHelpersGroup.add(cylinder);
      vectors.push(cylinder);
  }
}

//Crea nuevo vector de impacto
function updateHitVector() {
  
    const valX = parseInt(hitX.value, 10)*(-1);
    const valY = parseInt(hitY.value, 10)*(-1);
    const valZ = parseInt(hitZ.value, 10)*(-1);
    console.log(valX,valY,valZ);

    const hitPoint = new THREE.Vector3(valX, valY, valZ);
    const origin = new THREE.Vector3(0, 0, 0);
    const direction = new THREE.Vector3().subVectors(origin, hitPoint).normalize();

    // If hitArrowHelper exists, remove it from the scene
    if (hitArrowHelper) {
      scene.remove(hitArrowHelper);
    }

    hitArrowHelper = new THREE.ArrowHelper(
      direction,
      origin,
      hitPoint.distanceTo(origin),
      0xffff00
    );
    hitArrowHelper.userData = { dirVector: hitPoint};
    scene.add(hitArrowHelper);
  }
  updateHitVector();
  document.getElementById("updateHitVector").addEventListener("click", updateHitVector);


  function accionarMotorX(element){
    var xhr = new XMLHttpRequest();
    let number;
    switch (element.id) {
        case 'Sup':
            number = numPower1.textContent;
            break;
        case 'mNorte':
            number = numPower2.textContent;
            break;
        case 'mSur':
            number = numPower3.textContent;
            break;
        case 'mEste':
            number = numPower4.textContent;
            break;
        case 'mOeste':
            number = numPower5.textContent;
            break;
        case 'Inf':
            number = numPower6.textContent;
            break;
        default:
            Serial.println("Solenoide inválido");
            break;
    }
    xhr.open("GET", "/"+element.id+ "?number=" + number, true);
    console.log(element.id+number);
    xhr.send();
}



//**********************************PRUEBAS******************************************************

  function launch(values){
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/launch"+"?values=" + values, true);
    console.log(element.id+number);
    xhr.send();
}

function getMotorValues(x, y, z) {
    const impactVector = new THREE.Vector3(x, y, z).normalize();
    const projections = referenceVectors.map(vec => impactVector.dot(vec));
    //Convertir las proyecciones en valores de 0 a 255
    const values = projections.map(proj => Math.max(0, Math.min(255, Math.round(proj * 255))));

    launch(values);
}



function animateVectorsToNewCoordinates() {
    console.log('LAUNCH');
    const valX = parseInt(hitX.value, 10)*(-1);
    const valY = parseInt(hitY.value, 10)*(-1);
    const valZ = parseInt(hitZ.value, 10)*(-1);

    const x = valX;
    const y = valY;
    const z = valZ;

    //Enviar señal al esp32
    activateMotores(x,y,z);
    
    const components = [
        new THREE.Vector3(x, 0, 0),
        new THREE.Vector3(0, y, 0),
        new THREE.Vector3(0, 0, z)
    ];

    components.forEach((component, index) => {
        const targetLength = component.length();
        console.log('target length'+ targetLength);
        const scaleFactor = targetLength / 2.5;
        console.log('scaleFactor'+ scaleFactor);
    
        let vector;
        if (index === 0) { // X direction
            if (x >= 0) {
                vector = vectors[0];
            } else {
                vector = vectors[1];
            }
        } else if (index === 1) { // Y direction
            if (y >= 0) {
                vector = vectors[2];
            } else {
                vector = vectors[3];
            }
        } else if (index === 2) { // Z direction
            if (z >= 0) {
                vector = vectors[4];
            } else {
                vector = vectors[5];
            }
        }

        vector.scale.set(scaleFactor, 1, 1);

    });
}

function activateMotores(valX,valY,valZ){
    
    const sup = 0;
    const inf= 0;
    const norte= 0;
    const este= 0;
    const oeste= 0;
    const sur= 0;
    const values=[];

    var xhr = new XMLHttpRequest();
    //If look check is deactivated
    if(loopCheck){
      //launch con periodicidad
      xhr.open("GET", "/launch"+"?values=" + values+ "?sleep=" + sleep, true);
    }
    //launch sin periodicidad
    else{
      if (valY>0){
          sup = valY;
      }
      else{
          inf = valY;
      }
      if (valX>0){
          norte = valX;
      }
      else{
          sur = valX;
      } 
      if (valZ>0){
          este = valZ;
      }
      else{
          oeste = valZ;
      } 
      values=[sup,inf,norte,sur,este,oeste];
      //launch values;
      xhr.open("GET", "/launch"+"?values=" + values, true);
      console.log(element.id+number);
      xhr.send();
    }
    
}

/*
function activateLaunch(){
    const valX = parseInt(hitX.value, 10)*(-1);
    const valY = parseInt(hitY.value, 10)*(-1);
    const valZ = parseInt(hitZ.value, 10)*(-1);

    const hitPoint = new THREE.Vector3(valX, valY, valZ);
    const origin = new THREE.Vector3(0, 0, 0);
    const direction = new THREE.Vector3().subVectors(origin, hitPoint).normalize();
    const normalizedImpact = direction.clone().normalize();
    const projections = referenceVectors.map(vec => normalizedImpact.dot(vec)); //proyección 
    const values = projections.map(proj => Math.max(0, Math.min(255, Math.round(proj * 255))));
    //launch(values);
    for (let i = 0; i < vectors.length; i++) {
      const cylinder = vectors[i];
      const value = values[i];
      const scale = value / 255;
      cylinder.scale.set(1, scale, 1);
      const color = new THREE.Color(`rgb(${value}, 0, 0)`);
      cylinder.material.color.set(color);
  }
}*/

)=====";