R"=====(
html {
    font-family: Arial;
    display: inline-block;
    text-align: center;
  }
  p {
    font-size: 1.2rem;
  }
  body {
    margin: 0;
  }
  .banner {
    overflow: hidden;
    background-color: rgb(28, 169, 235);
    color:rgb(252, 252, 252);
    font-size: 1rem;
    margin-left: 38%;
    margin-right: 38%;
  }
  .content {
    padding: 20px;
  }
  .card {
    background-color: rgb(182, 176, 176);
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
  }
.cardSliders {
  background-color: rgb(182, 176, 176);
  box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 10px; /* Espacio entre el botón y el slider */
}

.fixed-width-span {
  display: inline-block;
  min-width: 30px; /* Ajusta el ancho mínimo según sea necesario */
  text-align: center; /* Alinea el contenido del span al centro */
}

.button-slider-pair {
  display: flex;
  align-items: center;
  gap: 10px; /* Espacio entre el botón y el slider */
}
  .card-title {
    color:rgb(0, 95, 173);
    font-weight: bold;
  }
  .cards {
    margin: 0 auto;
    display: grid; grid-gap: 2rem;
    /*grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
    display: grid; grid-gap: 2rem;  */
    /*grid-template-columns: 1fr 1fr;*/
    grid-template-columns: 1fr 1fr 1fr 1fr 2fr  ;
  }
  .reading {
    font-size: 1.2rem;
  }
  .sphere-content{
    color: grey;
    width: 100%;
    background-color: white;
    height: 300px; margin: auto;
    padding-top:2%;
  }
  #reset,#updateHitVector,#launch,#hitBtn{
    border: none;
    color: #f7f3f3;
    font-weight: bold;
    background-color: rgb(0, 95, 173);
    padding: 9px;
    text-align: center;
    display: inline-block;
    font-size: 14px; width: 150px;
    border-radius: 4px;
  }
  #Sup,#Inf,#mNorte,#mSur,#mEste,#mOeste{
    border: none;
    color: #FEFCFB;
    background-color: #1673cf;
    padding: 9px;
    text-align: center;
    display: inline-block;
    font-size: 14px; width: 150px; height: 25px;
    border-radius: 4px;
  }


)=====";
