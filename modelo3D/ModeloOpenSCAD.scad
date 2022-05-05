$fn = 20;

// Patas parte inferior
alturaPatas = 20;
color("Black"){
    translate([10, 10, -20])
    cube([10, 10, 20]);
    translate([230, 10, -20])
    cube([10, 10, 20]);
    translate([10, 230, -20])
    cube([10, 10, 20]);
    translate([230, 230, -20])
    cube([10, 10, 20]);
}

// Base cuadrada parte inferior
alturaBaseCuadrada = 10;
dimBaseCuadrada = 250;
color("Green"){
    cube([dimBaseCuadrada, dimBaseCuadrada, alturaBaseCuadrada]);
}


// Base turquesa
alturaBaseTurquesa = 5;
dimBaseTurquesa = 250;
color("DarkTurquoise"){
    translate([dimBaseTurquesa / 2, dimBaseTurquesa / 2, alturaBaseCuadrada])
    cylinder(h = alturaBaseTurquesa, d = dimBaseTurquesa);
}


// Cilindros base turquesa -> base verde
alturaCilindros = 80;
grosorCilindros = 10;
translate([dimBaseTurquesa / 2, dimBaseTurquesa / 2, alturaBaseCuadrada + alturaBaseTurquesa])
color("White"){
    translate([-60, -60, 0])
    cylinder(h = alturaCilindros, d = grosorCilindros);
    difference() {
        translate([-60, 60, 0])
        cylinder(h = alturaCilindros, d = grosorCilindros);
        translate([-60, 60, 5])
        cylinder(h = alturaCilindros - 4, d = grosorCilindros - 4);
    }
    translate([60, -60, 0])
    cylinder(h = alturaCilindros, d = grosorCilindros);
    translate([60, 60, 0])
    cylinder(h = alturaCilindros, d = grosorCilindros);
}

// Base verde superior
// Se incluye corte para mostrar hueco taladrado para pasar cables
translate([dimBaseTurquesa / 2, dimBaseTurquesa / 2, alturaBaseCuadrada + alturaBaseTurquesa + alturaCilindros])
color("Green"){
    difference() {
        cylinder(h = alturaBaseTurquesa, d = 200);
        translate([-60, 60, -1])
        color("Red")
        cylinder(h = 20, d = grosorCilindros);
    }
}

// Caja de cartas
alturaCaja = 20;
translate([dimBaseTurquesa / 2, dimBaseTurquesa / 2, alturaBaseCuadrada + alturaBaseTurquesa + alturaCilindros + alturaCaja / 2])
color("DarkTurquoise")
difference(){
    cube([92, 62, alturaCaja], center = true);
    translate([-2, 0, alturaBaseTurquesa + 1])
    cube([95, 60, alturaCaja], center = true);
}

// Tubos para levantar motor
translate([dimBaseTurquesa / 2, dimBaseTurquesa / 2, alturaBaseCuadrada + alturaBaseTurquesa + alturaCilindros])
color("White"){
    translate([-20, 37, 0])
    cylinder(h = alturaCaja, d = grosorCilindros);
}

// Tapa superior
translate([dimBaseTurquesa / 2, dimBaseTurquesa / 2, alturaBaseCuadrada + alturaBaseTurquesa + alturaCilindros + alturaCaja - 2])
difference(){
    color("Gold")
    cube([92, 58, 2], center = true);
    translate([-46, -29, -1])
    color("White")
    cube([55, 38, 4]);
}

// Goma giratoria
translate([dimBaseTurquesa / 2, dimBaseTurquesa / 2, alturaBaseCuadrada + alturaBaseTurquesa + alturaCilindros + alturaCaja / 2 + 2])
translate([-30, 8, 0])
rotate([90, 90, 0])
color("Black")
difference(){
    cylinder(h = 1, d = alturaCaja / 2);
    cylinder(h = 1, d = alturaCaja / 2 - 3);
}