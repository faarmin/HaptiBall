R"=====(

import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";

//Variables escena
var scene,
  camera,
  renderer,
  sphere,
  hitArrowHelper,
  arrowHelpersGroup,
  pointsGroup,
  slider,
  cylinderHelpersGroup;
//Variables sliders
var numPower1,
  numPower2, 
  numPower3, 
  numPower4, 
  numPower5, 
  numPower6,
  slider1,
  slider2,
  slider3,
  slider4,
  slider5,
  slider6;
var repsInput,
  actTime,
  waitTime;
var numHitX, 
  numHitY, 
  numHitZ,
  hitX,
  hitY,
  hitZ;
var magX, magY, magZ;
//Checks 
var fibonacciAct, 
  regAct, 
  configureHit;
//Los controles son globales 
var controls; 
//Arrays
const vectors = [];
var decomposedVectors = [];
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
    scene.background = new THREE.Color("black");

  camera = new THREE.PerspectiveCamera(
    75,
    parentWidth(document.getElementById("3Dcube")) /
      parentHeight(document.getElementById("3Dcube")),
    0.1,
    1000
  );

   // Iñigo: Reemplazo la camara por una ortográfica
  let width = parentWidth(document.getElementById("3Dcube"));
  let height = parentHeight(document.getElementById("3Dcube"));
  let sizeRatio = width / height; // mantengo el ratio de aspecto que tenia en pantalla
  let desiredWith = 30;
  camera = new THREE.OrthographicCamera(
    -desiredWith / 2,
    desiredWith / 2,
    desiredWith / sizeRatio / 2,
    desiredWith / sizeRatio / -2,
    0,
    1000
  );
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")));

    document.getElementById('3Dcube').appendChild(renderer.domElement);
    // Iñigo: añado orbit controls
    controls = new OrbitControls(camera, renderer.domElement);
    controls.update();

    // Crear la geometría
    const sphereGeometry = new THREE.SphereGeometry(2.5, 32, 32);
    const sphereMaterial = new THREE.MeshBasicMaterial({
      color: 0x8a8bed, // Iñigo: Cambio el color de la esfera a uno mas sauve que permita ver los ejes de su interior
      wireframe: true,
      opacity: 0.025,
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

  camera.position.x = 11; // Iñigo: alejo la camara lo suficiente para ver los valores máximos
  camera.position.y = 11; // Iñigo: alejo la camara lo suficiente para ver los valores máximos
  camera.position.z = 11; // Iñigo: alejo la camara lo suficiente para ver los valores máximos
  controls.update(); // Iñigo: Añado update de los controles
  renderer.render(scene, camera);


}

// Ajuste de tamaño responsive
function onWindowResize(){
    console.log('WindowResize');
    camera.aspect = parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube"));
    camera.updateProjectionMatrix();
    renderer.setSize(parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")));
    
}

//Reset de la esfera (posicion inicial vertical)
function resetPosition(){
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/reset", true);
    console.log('Reset');
    xhr.send();
}

/************************************************************************************************************************
*************************************************************************************************************************
************************************************-EN MARCHA-*********************************************************************/

window.addEventListener('resize', onWindowResize, false);

// Create the 3D representation
createSceneJS();
fibonacciAct= false;
regAct = false;



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
document.getElementById('reset').addEventListener('click', resetPosition);

document.getElementById("Sup").addEventListener('click', function() {
    accionarMotorX('Sup');
});

document.getElementById("mNorte").addEventListener('click', function() {
    accionarMotorX('mNorte');
});

document.getElementById("mSur").addEventListener('click', function() {
    accionarMotorX('mSur');
});

document.getElementById("mEste").addEventListener('click', function() {
    accionarMotorX('mEste');
});

document.getElementById("mOeste").addEventListener('click', function() {
    accionarMotorX('mOeste');
});

document.getElementById("Inf").addEventListener('click', function() {
    accionarMotorX('Inf');
});

document.getElementById("hitBtn").addEventListener("click", hitDecomposition);

document.getElementById("launch").addEventListener("click", activateMotores);

document.getElementById("hideVectorsBtn").addEventListener('click', hideVectors);

document.getElementById('myCheckbox').addEventListener('change', function() {
    fibonacciAct = this.checked;
    arrowHelpersGroup.visible = fibonacciAct;
    pointsGroup.visible = fibonacciAct;
    if(fibonacciAct){
      distributePointsOnSphere(6);
    }   
    console.log('Checkbox Fibonacci activado:', fibonacciAct);
});

document.getElementById('myCheckbox2').addEventListener('change', function() {
    regAct = this.checked;
    cylinderHelpersGroup.visible = regAct;
    pointsGroup.visible = regAct;
    if (regAct){
      createVectors();
    }
    console.log('Checkbox Regular distribution activado:', regAct);

});

document.getElementById('configureHit').addEventListener('change', function() {
    configureHit = this.checked;
    console.log('Checkbox Custom Hit activado:', configureHit);
});

document.addEventListener("DOMContentLoaded", () => {
  //Update Sliders
  let hitXSlider = document.getElementById("hitX");
  let hitXnum = document.getElementById("numHitX");
  hitXSlider.addEventListener("input", () => {
    hitXnum.innerHTML = hitX.value;
    updateHitVector();
  });

  let hitYSlider = document.getElementById("hitY");
  let hitYnum = document.getElementById("numHitY");
  hitYSlider.addEventListener("input", () => {
    hitYnum.innerHTML = hitY.value;
    updateHitVector();
  });

  let hitZSlider = document.getElementById("hitZ");
  let hitZnum = document.getElementById("numHitZ");
  hitZSlider.addEventListener("input", () => {
    hitZnum.innerHTML = hitZ.value;
    updateHitVector();
  });
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

    // Change sphere rotation 
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
  //[HACER QUE SEA SI SE ACTIVA LA VARIABLE]
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
  //console.log(valX,valY,valZ);

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


function accionarMotorX(motor){
  var xhr = new XMLHttpRequest();
  let number;
  switch (motor) {
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
  }

  if(configureHit){
    repsInput = document.getElementById("reps").value;
    waitTime = document.getElementById("waitTime").value;
    actTime = document.getElementById("actTime").value;

    console.log("WAITTIME: "+waitTime+" ACTTIME: "+actTime+ " REPETITIONS: "+repsInput);
    xhr.open("GET", "/"+motor+ "?number=" + number + "&actTime=" + actTime + "&waitTime=" + waitTime + "&repsInput=" + repsInput, true);
  }
  else{
    xhr.open("GET", "/"+motor+ "?number=" + number, true);
    console.log(motor+number);
  }
  xhr.send();
}


function activateMotores(){
    hitDecomposition();
    const valX = magX*(-1);
    const valY = magY*(-1);
    const valZ = magZ*(-1);
    let  sup = 0;
    let  inf= 0;
    let  norte= 0;
    let  este= 0;
    let  oeste= 0;
    let  sur= 0;
    let  values = [];

    var xhr = new XMLHttpRequest();
      if (valY>0){
          sup = valY;
      }
      else{
          inf = valY*(-1);
      }
      if (valX>0){
          norte = valX;
      }
      else{
          sur = valX*(-1);
      } 
      if (valZ>0){
          este = valZ;
      }
      else{
          oeste = valZ*(-1);
      } 
      values=[sup,inf,norte,sur,este,oeste];

      console.log(values);

    if(configureHit){
      repsInput = document.getElementById("reps").value;
      waitTime = document.getElementById("waitTime").value;
      actTime = document.getElementById("actTime").value;
      //launch con periodicidad
      console.log("WAITTIME: "+waitTime+" ACTTIME: "+actTime+ " REPETITIONS: "+repsInput);
      xhr.open("GET", "/customLaunch?values=" + values + "&actTime=" + actTime + "&waitTime=" + waitTime + "&repsInput=" + repsInput, true);
      xhr.send();
    }
    else{
      xhr.open("GET", "/launch"+"?values=" + values, true);
      xhr.send();
    }
    
}

function hideVectors() {
    decomposedVectors.forEach(vector => {
        scene.remove(vector); 
    });
    decomposedVectors = [];
    renderer.render(scene, camera);
}

//******* AÑADIDOS DE IÑIGO    *********/
// He cambiado a sistema de modulos para pode rimprotar orbit contorls
// Importo orbitcontrols
// He cambiado muchas variables a globales para poder acceder a ellas desde cualquier parte
// He cambiado color de esfera a uno más suave para poder ver los ejes de su interior
// cambio a orthographic camera (https://threejs.org/docs/#api/en/cameras/OrthographicCamera)


// Añado ejes de ayuda dentro de la esfera (en lugar de cilindros). Necesito esta referencia visual para conocer el eje de coordenadas local de la esfera conforme rota.
// Representan los solenoides.
const solenoidsAxesHelper = new THREE.AxesHelper(1);
sphere.add(solenoidsAxesHelper);

// añado tambien ejes generales de referencia de toda la escena
const sceneAxesHelper = new THREE.AxesHelper(10); // 10 veces mas largos, para diferenciarlos
sceneAxesHelper.setColors(0xfaafaf, 0xa1ffa1, 0xc4ddff); // Con colores mas sauves para diferenciarlos aun mejor
scene.add(sceneAxesHelper);


// Función que se encarga de descomponer y dibujar el hit
function hitDecomposition() {
  // Recibe un vector con magnitud y un color, dibuja un arrowHelper que lo visualiza en escena.
  function visualizeComponent(vector, length, color) {
    vector = vector.negate();
    const arrowHelper = new THREE.ArrowHelper(
      vector.normalize(), // Dirección normalizada
      new THREE.Vector3(0, 0, 0), // Punto de origen
      Math.abs(length), // Longitud/magnitud. Siempre absoluta, queremos distancias positivas sea cual sea su dirección
      color, // Color
      0.2, // Tamaño de la cabeza
      0.1 // Tamaño de la cabeza
    );
    scene.add(arrowHelper);
    decomposedVectors.push(arrowHelper); 
  }

  // Obtengo la "world matrix" del solenoidsAxesHelper, la necesitaré para usarlo como base de la decomposición
  const axesHelperWorldMatrix = solenoidsAxesHelper.matrixWorld;

  // Extract the basis vectors (X, Y, Z) from the matrix
  const xAxis = new THREE.Vector3();
  const yAxis = new THREE.Vector3();
  const zAxis = new THREE.Vector3();
  axesHelperWorldMatrix.extractBasis(xAxis, yAxis, zAxis);


  // Construyo el vector de impacto a partir de los valores del hit input
  const hitPoint = new THREE.Vector3(
    parseInt(hitX.value),
    parseInt(hitY.value),
    parseInt(hitZ.value)
  );
  const origin = new THREE.Vector3(0, 0, 0);
  const direction = new THREE.Vector3()
    .subVectors(origin, hitPoint)
    .normalize(); //normalizado, la magnitud se la doy ahora justo después
  const hitArrowLength = hitPoint.distanceTo(origin); // la magnitud/longitud es la distancia al origen 0,0,0
  const hitVector = direction.clone().multiplyScalar(hitArrowLength);

  // Clono el vector de impacto para poder proyectarlo sobre los 3 ejes (internos de la esfera, corresponoden a solenoides)
  let compX = hitVector.clone().projectOnVector(xAxis);
  let compY = hitVector.clone().projectOnVector(yAxis);
  let compZ = hitVector.clone().projectOnVector(zAxis);
 

  // Obtengo la magnitud real de cada componente, para poder visualizarla
  // https://mathinsight.org/media/image/image/dot_product_projection.png
  // Una magnitud negativa significa que tengo que activar el solenoide opuesto
  const lengthX = hitVector.dot(xAxis); // Magnitud real de la componente X
  const lengthY = hitVector.dot(yAxis); // Magnitud real de la componente Y
  const lengthZ = hitVector.dot(zAxis); // Magnitud real de la componente Z
  magX=lengthX;
  magY=lengthY;
  magZ=lengthZ;
  console.log("Lengths [X: "+lengthX+" Y: "+lengthY+" Z: "+lengthZ+" ]");

  // Visualizo las componentes/
  visualizeComponent(compX, lengthX, 0xd43fbe);
  visualizeComponent(compY, lengthY, 0xd43fbe);
  visualizeComponent(compZ, lengthZ, 0xd43fbe);

  // Fuerzo el redibujado, por si estaba parado el renderer
  renderer.render(scene, camera);
}



)=====";