// Iñigo: Estoy tratando todo esto como un modulo para poder importar los orbitcontrolls
import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";

//Variables
var scene,
  camera,
  renderer,
  sphere,
  hitArrowHelper,
  arrowHelpersGroup,
  pointsGroup,
  slider,
  cylinderHelpersGroup;
var numPower1, numPower2, numPower3, numPower4, numPower5, numPower6;
var numHitX, numHitY, numHitZ;
var fibonacciAct, regAct, loopCheck;
var controls; // Iñigo: Los controles son globales para poder actualizarlos desde cualquier parte
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
function createSceneJS() {
  console.log("Entra a createSceneJS");

  scene = new THREE.Scene();
  scene.background = new THREE.Color(0xffffff);

  // camera = new THREE.PerspectiveCamera(75, parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube")), 0.1, 1000);
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
  renderer.setSize(
    parentWidth(document.getElementById("3Dcube")),
    parentHeight(document.getElementById("3Dcube"))
  );

  document.getElementById("3Dcube").appendChild(renderer.domElement);

  // Iñigo: añado orbit controls
  controls = new OrbitControls(camera, renderer.domElement);
  controls.update();

  // Crear la geometría
  const sphereGeometry = new THREE.SphereGeometry(2.5, 32, 32);
  const sphereMaterial = new THREE.MeshBasicMaterial({
    color: 0x8a8bed, // Iñigo: Cambio el color de la esfera a uno mas sauve que permita ver los ejes de su interior
    wireframe: true,
    opacity: 0.05,
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

  console.log("Sale createSceneJS");
}

// Ajuste de tamaño responsive
function onWindowResize() {
  console.log("WindowResize");
  camera.aspect =
    parentWidth(document.getElementById("3Dcube")) /
    parentHeight(document.getElementById("3Dcube"));
  camera.updateProjectionMatrix();
  renderer.setSize(
    parentWidth(document.getElementById("3Dcube")),
    parentHeight(document.getElementById("3Dcube"))
  );
}

//Reset de la esfera (posicion inicial vertical)
function resetPosition(element) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/" + element.id, true);
  console.log(element.id);
  xhr.send();
}

/************************************************************************************************************************
 *************************************************************************************************************************
 ************************************************-EN MARCHA-*********************************************************************/

window.addEventListener("resize", onWindowResize, false);

// Create the 3D representation
createSceneJS();
fibonacciAct = true;
//distributePointsOnSphere(6);
//createVectors();

//***********************DOCUMENT GETTERS********************
// Iñigo: Comento para evitar conflictos. Defino event listeners mas abajo
/*
document.addEventListener("DOMContentLoaded", () => {
  var slider1 = document.getElementById("slider1");
  var slider2 = document.getElementById("slider2");
  var slider3 = document.getElementById("slider3");
  var slider4 = document.getElementById("slider4");
  var slider5 = document.getElementById("slider5");
  var slider6 = document.getElementById("slider6");
  var numPower1 = document.getElementById("numPower1");
  var numPower2 = document.getElementById("numPower2");
  var numPower3 = document.getElementById("numPower3");
  var numPower4 = document.getElementById("numPower4");
  var numPower5 = document.getElementById("numPower5");
  var numPower6 = document.getElementById("numPower6");

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

  let hitXSlider = document.getElementById("hitX");
  const hitY = document.getElementById("hitY");
  const hitZ = document.getElementById("hitZ");

  var numHitX = document.getElementById("numHitX");
  var numHitY = document.getElementById("numHitY");
  var numHitZ = document.getElementById("numHitZ");

  hitXSlider.addEventListener("input", () => {
    console.log("New hitX value:");
  });
  hitX.oninput = function () {
    console.log("hitX.oninput");
    const n = parseInt(this.value);
    numHitX.textContent = n;
  };
  hitY.oninput = function () {
    console.log("hitY.oninput");
    const n = parseInt(this.value);
    numHitY.textContent = n;
  };

  hitZ.oninput = function () {
    console.log("hitZ.oninput");
    const n = parseInt(this.value);
    numHitZ.textContent = n;
  };
  
});
*/

//*****************************LISTENERS*********************
document.getElementById("myCheckbox").addEventListener("change", function () {
  fibonacciAct = this.checked;
  arrowHelpersGroup.visible = fibonacciAct;
  pointsGroup.visible = fibonacciAct;
  console.log("Checkbox activado:", fibonacciAct);
});

document.getElementById("myCheckbox2").addEventListener("change", function () {
  regAct = this.checked;
  cylinderHelpersGroup.visible = regAct;
  pointsGroup.visible = regAct;
  console.log("Checkbox activado:", regAct);
});

document.getElementById("loopCheck").addEventListener("change", function () {
  loopCheck = this.checked;
  console.log("Checkbox activado:", loopCheck);
});

// Create events for the sensor readings
/*
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
*/

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
    const pointGeometry = new THREE.SphereGeometry(0.01, 32, 32);
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
      point.position.length() * 2.5,
      0xff0000
    );
    arrowHelpersGroup.add(arrowHelper);
  }
}

//Crea vectores ortogonales
function createVectors() {
  const radius = 2.5; // Radius of the sphere
  const cylinderRadius = 0.025; // Radius of the cylinder
  const vectorLength = radius;

  // Create the 6 vectors
  const directions = [
    new THREE.Vector3(0, 1, 0),
    new THREE.Vector3(0, -1, 0),
    new THREE.Vector3(1, 0, 0),
    new THREE.Vector3(-1, 0, 0),
    new THREE.Vector3(0, 0, 1),
    new THREE.Vector3(0, 0, -1),
  ];

  for (const direction of directions) {
    const cylinderGeometry = new THREE.CylinderGeometry(
      cylinderRadius,
      cylinderRadius,
      vectorLength,
      16
    );
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
  const valX = parseInt(hitX.value, 10) * -1;
  const valY = parseInt(hitY.value, 10) * -1;
  const valZ = parseInt(hitZ.value, 10) * -1;
  console.log(valX, valY, valZ);

  const hitPoint = new THREE.Vector3(valX, valY, valZ);
  const origin = new THREE.Vector3(0, 0, 0);
  const direction = new THREE.Vector3()
    .subVectors(origin, hitPoint)
    .normalize();

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
  hitArrowHelper.userData = { dirVector: hitPoint };
  scene.add(hitArrowHelper);
}
updateHitVector();
document
  .getElementById("updateHitVector")
  .addEventListener("click", updateHitVector);

function accionarMotorX(element) {
  var xhr = new XMLHttpRequest();
  let number;
  switch (element.id) {
    case "Sup":
      number = numPower1.textContent;
      break;
    case "mNorte":
      number = numPower2.textContent;
      break;
    case "mSur":
      number = numPower3.textContent;
      break;
    case "mEste":
      number = numPower4.textContent;
      break;
    case "mOeste":
      number = numPower5.textContent;
      break;
    case "Inf":
      number = numPower6.textContent;
      break;
    default:
      Serial.println("Solenoide inválido");
      break;
  }
  xhr.open("GET", "/" + element.id + "?number=" + number, true);
  console.log(element.id + number);
  xhr.send();
}

//**********************************PRUEBAS******************************************************

function launch(values) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/launch" + "?values=" + values, true);
  console.log(element.id + number);
  xhr.send();
}

function getMotorValues(x, y, z) {
  const impactVector = new THREE.Vector3(x, y, z).normalize();
  const projections = referenceVectors.map((vec) => impactVector.dot(vec));
  //Convertir las proyecciones en valores de 0 a 255
  const values = projections.map((proj) =>
    Math.max(0, Math.min(255, Math.round(proj * 255)))
  );

  launch(values);
}

function animateVectorsToNewCoordinates() {
  console.log("LAUNCH");
  const valX = parseInt(hitX.value, 10) * -1;
  const valY = parseInt(hitY.value, 10) * -1;
  const valZ = parseInt(hitZ.value, 10) * -1;

  const x = valX;
  const y = valY;
  const z = valZ;

  //Enviar señal al esp32
  activateMotores(x, y, z);

  const components = [
    new THREE.Vector3(x, 0, 0),
    new THREE.Vector3(0, y, 0),
    new THREE.Vector3(0, 0, z),
  ];

  components.forEach((component, index) => {
    const targetLength = component.length();
    console.log("target length" + targetLength);
    const scaleFactor = targetLength / 2.5;
    console.log("scaleFactor" + scaleFactor);

    let vector;
    if (index === 0) {
      // X direction
      if (x >= 0) {
        vector = vectors[0];
      } else {
        vector = vectors[1];
      }
    } else if (index === 1) {
      // Y direction
      if (y >= 0) {
        vector = vectors[2];
      } else {
        vector = vectors[3];
      }
    } else if (index === 2) {
      // Z direction
      if (z >= 0) {
        vector = vectors[4];
      } else {
        vector = vectors[5];
      }
    }

    vector.scale.set(scaleFactor, 1, 1);
  });
}

function activateMotores(valX, valY, valZ) {
  const sup = 0;
  const inf = 0;
  const norte = 0;
  const este = 0;
  const oeste = 0;
  const sur = 0;
  const values = [];

  var xhr = new XMLHttpRequest();
  //If look check is deactivated
  if (loopCheck) {
    //launch con periodicidad
    xhr.open("GET", "/launch" + "?values=" + values + "?sleep=" + sleep, true);
  }
  //launch sin periodicidad
  else {
    if (valY > 0) {
      sup = valY;
    } else {
      inf = valY;
    }
    if (valX > 0) {
      norte = valX;
    } else {
      sur = valX;
    }
    if (valZ > 0) {
      este = valZ;
    } else {
      oeste = valZ;
    }
    values = [sup, inf, norte, sur, este, oeste];
    //launch values;
    xhr.open("GET", "/launch" + "?values=" + values, true);
    console.log(element.id + number);
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

//******* AÑADIDOS DE IÑIGO    *********/
// He cambiado a sistema de modulos para pode rimprotar orbit contorls
// Importo orbitcontrols
// He cambiado muchas variables a globales para poder acceder a ellas desde cualquier parte
// He cambiado color de esfera a uno más suave para poder ver los ejes de su interior
// cambio a orthographic camera (https://threejs.org/docs/#api/en/cameras/OrthographicCamera)

// Añado un botón de pausa para reanudar/detener la rotación de la esfera
let rotationActivated = true;
function stopResumeRotation() {
  rotationActivated = !rotationActivated;
}
//Añado botones de pausa/continuar rotación y de simular HIT.
// Tambien listeners de sliders impact vector
document.addEventListener("DOMContentLoaded", () => {
  let hitVectorPanel = document.getElementById("vectorHit");
  hitVectorPanel.innerHTML =
    hitVectorPanel.innerHTML +
    '<button id="stopResumeRot">Stop/resume rotation</button></br>';
  hitVectorPanel.innerHTML =
    hitVectorPanel.innerHTML +
    '</br></br><button id="hitBtn">Hit</button></br>';
  document
    .getElementById("stopResumeRot")
    .addEventListener("click", stopResumeRotation);
  document.getElementById("hitBtn").addEventListener("click", hitDecomposition);

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

//  Comento todo el código relativo a dibujado de ejes mediante cilindros y distribución de puntos.
// Añado ejes de ayuda dentro de la esfera (en lugar de cilindros). Necesito esta referencia visual para conocer el eje de coordenadas local de la esfera conforme rota.
// Representan los solenoides.
const solenoidsAxesHelper = new THREE.AxesHelper(1);
sphere.add(solenoidsAxesHelper);

// añado tambien ejes generales de referencia de toda la escena
const sceneAxesHelper = new THREE.AxesHelper(10); // 10 veces mas largos, para diferenciarlos
sceneAxesHelper.setColors(0xfaafaf, 0xa1ffa1, 0xc4ddff); // Con colores mas sauves para diferenciarlos aun mejor
scene.add(sceneAxesHelper);

// Fondo a negro, para ver mejor
scene.background = new THREE.Color("black");

// Función que s encarga de descomponer y dibujar el hit
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
  }

  // Obtengo la "world matrix" del solenoidsAxesHelper, la necesitaré para usarlo como base de la decomposición
  const axesHelperWorldMatrix = solenoidsAxesHelper.matrixWorld;

  // Extract the basis vectors (X, Y, Z) from the matrix
  const xAxis = new THREE.Vector3();
  const yAxis = new THREE.Vector3();
  const zAxis = new THREE.Vector3();
  axesHelperWorldMatrix.extractBasis(xAxis, yAxis, zAxis);

  /*
  console.log("X-Axis Vector:", xAxis);
  console.log("Y-Axis Vector:", yAxis);
  console.log("Z-Axis Vector:", zAxis);
  */

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

  // Visualizo las componentes
  visualizeComponent(compX, lengthX, 0xff0000);
  visualizeComponent(compY, lengthY, 0x00ff00);
  visualizeComponent(compZ, lengthZ, 0x0000ff);

  // Fuerzo el redibujado, por si estaba parado el renderer
  renderer.render(scene, camera);
}

// Comento todo el código referente a evento de giroscopio
// Roto la esfera para simular el giroscopio
function animateSphere(sphere, speedX = 0.01, speedY = 0.01, speedZ = 0.01) {
  requestAnimationFrame(() => animateSphere(sphere, speedX, speedY, speedZ));
  if (rotationActivated) {
    // Rotar solo si la rotación está activada
    sphere.rotation.x += speedX;
    sphere.rotation.y += speedY;
    sphere.rotation.z += speedZ;
  }
  controls.update();
  renderer.render(scene, camera);
}
animateSphere(sphere);
