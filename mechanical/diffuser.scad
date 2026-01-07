//diffuser module for HTP system

use <fillets_and_rounds.scad>

module diffuser(x=300,y=350,z=20,xn=3,yn=4,face_th=0.4,wall_th=10,m=6,arch_depth=10,use_f_values=false,R=30){
    sqx = (x-(xn+1)*wall_th)/xn;
    sqy = (y-(yn+1)*wall_th)/yn;
    echo(sqx,sqy);
    difference(){
        cube([x,y,z]);
        //cutouts + board screw holes
        translate([wall_th,wall_th])for(i=[0:xn-1])for(j=[0:yn-1])
            translate([i*(sqx+wall_th),j*(sqy+wall_th),+face_th]){
                add_rounds(axis="z",R=R,fn=30)cube([sqx,sqy,z]);
                translate([(sqx-90)/2,(sqy-80)/2,z])led_motor_board(5);
            }
        
        //screw_holes
        f_th = 15;
        fx = (x-(xn+1)*f_th)/xn;
        fy = (y-(yn+1)*f_th)/yn;
        *translate([f_th/2,f_th/2])for(i=[0:xn])for(j=[0:yn])
            translate([i*(fx+f_th),j*(fy+f_th),face_th])cylinder(d=m,h=z);
        
        smx = 2; //screw margin x
        smy = 3; //screw margin y
        translate([wall_th/2,wall_th/2])for(i=[0:xn])for(j=[0:yn]){
            temp_x = i*(sqx+wall_th);
            temp_y = j*(sqy+wall_th);
            
            shift_x = 
            (i==0) ? temp_x+smx : 
            (i==xn) ? temp_x-smx : temp_x;
            
            shift_y = 
            (j==0) ? temp_y+smy : 
            (j==yn) ? temp_y-smy : temp_y;
            //echo(shift_x)
            translate([shift_x,shift_y,face_th])cylinder(d=m,h=z);
        }
        /*
        //grid of boards for screw holes
        bmx = 2; //board margin x
        bmy = 3; //board margin y
        b_thx = 6.5;
        b_thy = 5;
        translate([b_thx/2,b_thy/2+1,z])for(i=[0:xn-1])for(j=[0:yn-1]){
            temp_x = i*(sqx+b_thx);
            temp_y = j*(sqy+b_thy);
            
            bshift_x = 
            (i==0) ? temp_x+bmx : 
            (i==xn-1) ? temp_x-bmx : temp_x;
            
            bshift_y = 
            (j==0) ? temp_y+bmy : 
            (j==yn-1) ? temp_y-bmy : temp_y;
            //echo(shift_x)
            translate([bshift_x,bshift_y,face_th])led_motor_board();
        }*/
            
        //arch cuts
        outer_wall = wall_th;
        for(i=[0:xn-1])translate([i*(sqx+wall_th)+wall_th,outer_wall,z-arch_depth])
            add_rounds(axis="y",R=40,fn=30)cube([sqx,y-2*outer_wall,sqx]);
        for(i=[0:yn-1])translate([outer_wall,i*(sqy+wall_th)+wall_th,z-arch_depth])
        add_rounds(axis="x",R=38,fn=30)
        cube([x-2*outer_wall,sqy,sqy]);
    }//difference
}//diffuser

module led_motor_board(border=5,x=90,y=80){
    color("green",0.8)cube([x,y,2]);
    for(dx=[border:x-2*border:x-border])for(dy=[border:y-2*border:y-border]){translate([dx,dy])cylinder(d=3.3,h=10,center=true);}
}

diffuser(wall_th=4,R=35,arch_depth=-1);
//translate([7,7,20])led_motor_board();
//translate([95+10,82.5+9,20])led_motor_board();}