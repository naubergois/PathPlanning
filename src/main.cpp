#include <uWS/uWS.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "helpers.h"
#include "json.hpp"
#include "spline.h"

// for convenience
using nlohmann::json;
using std::string;
using std::vector;

void increase_velocity(double &ref_vel,double &car_speed){



          if(ref_vel < 30) //If path ahead us is clear, accelerate to reference velocity 
          {
             
             if ((ref_vel-car_speed)<0.400){
             	ref_vel += .224*10; }
	     else{
                std::cout<<" same velocity "<<std::endl; }	 

            // std::cout<<"Speed "<<car_speed<<std::endl;


             //std::cout<<"ref_vel "<<ref_vel<<std::endl;
          }   
          else if((ref_vel >= 30) && (ref_vel <40)) //If path ahead us is clear, accelerate to reference velocity 
          {
             
             if ((ref_vel-car_speed)<0.400){
             	ref_vel += .224; }
	     else{
                std::cout<<" same velocity "<<std::endl; }	 

             //std::cout<<"Speed "<<car_speed<<std::endl;


             //std::cout<<"ref_vel "<<ref_vel<<std::endl;
          }

           else if((ref_vel >= 40) && (ref_vel <45)) //If path ahead us is clear, accelerate to reference velocity 
          {
             
             if ((ref_vel-car_speed)<0.400){
             	ref_vel += .001; }
	     else{
                std::cout<<" same velocity "<<std::endl; }	 

             //std::cout<<"Speed "<<car_speed<<std::endl;


             //std::cout<<"ref_vel "<<ref_vel<<std::endl;
          }

           else if((ref_vel >= 45) && (ref_vel <49)) //If path ahead us is clear, accelerate to reference velocity 
          {
             
             if ((ref_vel-car_speed)<0.400){
             	ref_vel += .001; }
	     else{
                std::cout<<" same velocity "<<std::endl; }	 

             //std::cout<<"Speed "<<car_speed<<std::endl;


             //std::cout<<"ref_vel "<<ref_vel<<std::endl;
          }  




}




void decrease_velocity(double &ref_vel,double &car_speed){

          //std::cout<<" decrease velocity "<<std::endl; 

          if(ref_vel < 30) //If path ahead us is clear, accelerate to reference velocity 
          {
             
             if ((car_speed-ref_vel)<0.600){
             	ref_vel -= .224*3; }
	     else{
                std::cout<<" same velocity "<<std::endl; }	 

             //std::cout<<"Speed "<<car_speed<<std::endl;


             //std::cout<<"ref_vel "<<ref_vel<<std::endl;
          }   
          else if((ref_vel >= 30) && (ref_vel <40)) //If path ahead us is clear, accelerate to reference velocity 
          {
             
             if ((car_speed-ref_vel)<0.600){
             	ref_vel -= .124; }
	     else{
                std::cout<<" same velocity "<<std::endl; }	 

             //std::cout<<"Speed "<<car_speed<<std::endl;


             //std::cout<<"ref_vel "<<ref_vel<<std::endl;
          }

           else if((ref_vel >= 40) && (ref_vel <45)) //If path ahead us is clear, accelerate to reference velocity 
          {
             
             if ((car_speed-ref_vel)<0.500){
             	ref_vel -= .0001; }
	     else{
                std::cout<<" same velocity "<<std::endl; }	 

             //std::cout<<"Speed "<<car_speed<<std::endl;


             //std::cout<<"ref_vel "<<ref_vel<<std::endl;
          }  

          else if((ref_vel >= 45) && (ref_vel <49)) //If path ahead us is clear, accelerate to reference velocity 
          {
             
             if ((car_speed-ref_vel)<0.500){
             	ref_vel -= .0001; }
	     else{
                std::cout<<" same velocity "<<std::endl; }	 

             //std::cout<<"Speed "<<car_speed<<std::endl;


            // std::cout<<"ref_vel "<<ref_vel<<std::endl;
          }  





}

int main() {
  uWS::Hub h;

  // Load up map values for waypoint's x,y,s and d normalized normal vectors
  vector<double> map_waypoints_x;
  vector<double> map_waypoints_y;
  vector<double> map_waypoints_s;
  vector<double> map_waypoints_dx;
  vector<double> map_waypoints_dy;

  // Waypoint map to read from
  string map_file_ = "../data/highway_map.csv";
  // The max s value before wrapping around the track back to 0
  double max_s = 6945.554;

  std::ifstream in_map_(map_file_.c_str(), std::ifstream::in);

  string line;
  while (getline(in_map_, line)) {
    std::istringstream iss(line);
    double x;
    double y;
    float s;
    float d_x;
    float d_y;
    iss >> x;
    iss >> y;
    iss >> s;
    iss >> d_x;
    iss >> d_y;
    map_waypoints_x.push_back(x);
    map_waypoints_y.push_back(y);
    map_waypoints_s.push_back(s);
    map_waypoints_dx.push_back(d_x);
    map_waypoints_dy.push_back(d_y);
  }
  
  //start in lane 1 (middle lane)
  int lane = 1;
          
  //Have a reference velocity to target
  double ref_vel = 0.0; //Set initial speed to 0 mph
  
  h.onMessage([&ref_vel,&map_waypoints_x,&map_waypoints_y,&map_waypoints_s,
               &map_waypoints_dx,&map_waypoints_dy,&lane]
              (uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
               uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {

      auto s = hasData(data);

      if (s != "") {
        auto j = json::parse(s);
        
        string event = j[0].get<string>();
        
        if (event == "telemetry") {
          // j[1] is the data JSON object
          
          // Main car's localization Data
          double car_x = j[1]["x"];
          double car_y = j[1]["y"];
          double car_s = j[1]["s"];
          double car_d = j[1]["d"];
          double car_yaw = j[1]["yaw"];
          double car_speed = j[1]["speed"];

          // Previous path data given to the Planner
          auto previous_path_x = j[1]["previous_path_x"];
          auto previous_path_y = j[1]["previous_path_y"];
          // Previous path's end s and d values 
          double end_path_s = j[1]["end_path_s"];
          double end_path_d = j[1]["end_path_d"];

          // Sensor Fusion Data, a list of all other cars on the same side 
          //   of the road.
          auto sensor_fusion = j[1]["sensor_fusion"];

          json msgJson;
          
                  

          /**
           * TODO: define a path made up of (x,y) points that the car will visit
           *   sequentially every .02 seconds
           */

          int prev_size  = previous_path_x.size();  
          //previous path received from the simulator, which contains all the points from previous path, the vehicle didn't traverse

          //Stores the distance of vehicle ahead of us in the adjacent lanes to order to change to lane in which vehicle is further ahead  
          //double left_lane_vehicle_dist = 0.0;
          //double right_lane_vehicle_dist = 0.0;
          

          //PREDICTION STEP
          
          //Use sensor fusion data to detect other vehicles
          if(prev_size>0)
          {
            car_s = end_path_s;
          }
          
          bool ahead = false;
          bool left = false;
          bool right = false;
          int my_lane=0;


          if (car_d <= 4) {
            my_lane = 0; 
          }
          else if (car_d > 4 && car_d < 8 ) {
            my_lane = 1; 
          }
          else if (car_d >= 8) {
            my_lane = 2; 
          }
          
          
          int distance=30;
          
          int i=0;
          while(i<sensor_fusion.size())
          {
            float d = sensor_fusion[i][6];
            double vx = sensor_fusion[i][3];
            double vy = sensor_fusion[i][4];
            double check_speed = sqrt(vx*vx + vy*vy); 
            double check_car_s = sensor_fusion[i][5];
        
            check_car_s+= ((double)prev_size * 0.02 * check_speed); 


           



            if((check_car_s > car_s) && ((check_car_s - car_s) < distance)) 
            {
              if(d<(2+4*lane+2) && d>(2+4*lane-2)){
                  ahead = true;
                  //std::cout<<"car ahead "<<std::endl;
	      }
              if(d<(2+4*(lane-1)+2) && d>(2+4*(lane-1)-2)){  
                  left = true;
                  //std::cout<<"car left "<<std::endl;
              }
          
                
              if(d<(2+4*(lane+1)+2) && d>(2+4*(lane+1)-2)){ 
                  right = true;
                  //std::cout<<"car right "<<std::endl;
              }
             

             }  
             else if((check_car_s < car_s) && ((car_s - check_car_s) < distance))//For lane change check if car is behind and less than some buffer distance away
             {
              if(d<(2+4*(lane-1)+2) && d>(2+4*(lane-1)-2))  
                  left= true; 
              if(d<(2+4*(lane+1)+2) && d>(2+4*(lane+1)-2)) 
                  right= true;  
             }

             i=i+1;   
          } 
          
          //BEHAVIOR PLANNING 

          //std::cout<<"Lane "<<my_lane<<std::endl;

          bool check=true;
         
          if(ahead)  
          {  
           
            decrease_velocity(ref_vel,car_speed);  
            bool change_lane=false;
           
            if(!left && (lane - 1 >=0)  && ref_vel<40) //Left lane change is safe
            {
               lane--;
               change_lane=true;
               check=false;
            }
            else if(!right && (lane + 1 <=2) && ref_vel<40) //Right lane change is safe
            {
               lane++;
               change_lane=true;
               check=false;    
            } 
            if(!change_lane){

               //std::cout<<" car ahead- reduce velocity "<<std::endl;	

               

               ref_vel -= .124; 
               check=false;
            }else{
		//std::cout<<" change lane "<<std::endl;
                check=false;					
            }  
          }

	  else {
                 if (lane==0 && ref_vel<49 && ref_vel>45){
                    ref_vel += .124; 
                    check=false;
                 }
                 else if (lane==2 && ref_vel>30){
		    std::cout<<" decrease "<<std::endl;
                    decrease_velocity(ref_vel,car_speed);   
                 }
                 else if (lane==1 && ref_vel>37){
                    std::cout<<" decrease "<<std::endl;
                    decrease_velocity(ref_vel,car_speed);   
                 }    
		 else									
                    increase_velocity(ref_vel,car_speed);
                 
          }  




         

          if ((my_lane!=1)&&(check)&&(car_speed<35)&&(car_speed>=28)){

                      //std::cout<<" try to go center  "<<std::endl;	
                      bool change_lane=false; 
                      if(!left && (lane ==2)) //Left lane change is safe
                      {
                             
                           //std::cout<<" go left  "<<std::endl;   
                           decrease_velocity(ref_vel,car_speed);
                           lane--;
                           change_lane=true;
                      }
                      else if(!right && (lane==0)) //Right lane change is safe
                      {
                         //std::cout<<" go right  "<<std::endl;  
                         decrease_velocity(ref_vel,car_speed); 
                         lane++;
                         change_lane=true;
                      } 
                       


	   }


          if ((my_lane!=0)&&(check)&&(car_speed>=37)&&(car_speed<40)){

                      //std::cout<<" try to go left  "<<std::endl;	
                      bool change_lane=false; 
                      if(!left && (lane ==1)) //Left lane change is safe
                      {
                           //std::cout<<" go left  "<<std::endl;   
                           decrease_velocity(ref_vel,car_speed);
                           lane--;
                           change_lane=true;
                      }
                      else if(!left && (lane==2)) //Right lane change is safe
                      {
                           //std::cout<<" go left  "<<std::endl;   
                           decrease_velocity(ref_vel,car_speed);
                           lane--;
                           change_lane=true;
                      } 
                       


	   }
 
            
         
          
          // TRAJECTORY GENERATION
          
          //Create a list of widely spaced (x,y) waypoints, evenly spaced at 30m apart
          //later will use interpolate these waypoints with a spline and fill it in with more points that control speed
          
          vector<double> ptsx;
          vector<double> ptsy;
          
          // reference x,y, yaw states
          // either we will reference the starting point as where the car is or at the previous paths end point
          double ref_x = car_x;
          double ref_y = car_y;
          double ref_yaw = deg2rad(car_yaw);
          
          //if the previous size is almost empty, use the car as the staring reference
          if(prev_size<2)
          {
             //use two points that make the path tanget to the car
            double prev_car_x = car_x - cos(car_yaw);
            double prev_car_y = car_y - sin(car_yaw);
            
            ptsx.push_back(prev_car_x);
            ptsx.push_back(car_x);
            
            ptsy.push_back(prev_car_y);
            ptsy.push_back(car_y);
          }
          
          //use the previous path's end point as the starting reference
          else
          {
            //redefine the reference state as the previous path end point
            ref_x = previous_path_x[prev_size - 1];
            ref_y = previous_path_y[prev_size - 1];
            
            double ref_x_prev = previous_path_x[prev_size - 2];
            double ref_y_prev = previous_path_y[prev_size - 2];
            ref_yaw = atan2(ref_y - ref_y_prev, ref_x - ref_x_prev);
            
            //use two points that make the path tanget to the previous path's end point
            ptsx.push_back(ref_x_prev);
            ptsx.push_back(ref_x);
            
            ptsy.push_back(ref_y_prev);
            ptsy.push_back(ref_y);
          }
          
          //In Frenet add evenly spaced 30m spaced points ahead of the starting reference
          vector<double> next_wp0 = getXY(car_s + 30, (2+4*lane),map_waypoints_s,map_waypoints_x,map_waypoints_y);
          vector<double> next_wp1 = getXY(car_s + 60, (2+4*lane),map_waypoints_s,map_waypoints_x,map_waypoints_y);
          vector<double> next_wp2 = getXY(car_s + 90, (2+4*lane),map_waypoints_s,map_waypoints_x,map_waypoints_y);
          
          ptsx.push_back(next_wp0[0]);
          ptsx.push_back(next_wp1[0]);
          ptsx.push_back(next_wp2[0]);
          
          ptsy.push_back(next_wp0[1]);
          ptsy.push_back(next_wp1[1]);
          ptsy.push_back(next_wp2[1]);
          
          
          for(int i=0;i<ptsx.size();i++)
          {
            //shift car reference angle to 0 degrees
            //Transform the points to the local car co-ordinates,so that the first pt. in the path is at origin (0,0) and at heading of 0 degrees
            double shift_x = ptsx[i] - ref_x;
            double shift_y = ptsy[i] - ref_y;
            
            ptsx[i] = (shift_x * cos(0-ref_yaw) - shift_y*sin(0-ref_yaw));
            ptsy[i] = (shift_x * sin(0-ref_yaw) + shift_y*cos(0-ref_yaw));
          }
          
          //create a spline
          tk::spline s;
          
          //set(x,y) points to the spline
          s.set_points(ptsx,ptsy);
          
          //Define the actual (x,y) points we will use for the planner       
          vector<double> next_x_vals;
          vector<double> next_y_vals;
          
          //Start with all the previous path points from last time
          for(int i=0;i<previous_path_x.size();i++)
          {
            next_x_vals.push_back(previous_path_x[i]);
            next_y_vals.push_back(previous_path_y[i]);
          }
          
          //Calculate how to break up the spline points so that we travel at our desired refrence velocity
          double target_x = 30.0;
          double target_y = s(target_x);
          double target_dist = sqrt((target_x*target_x) + (target_y * target_y));
          
          double x_add_on = 0;
          
          //Fill up the rest of our path planner after filling it with previous points, here we will always have 50 points
          for(int i=1;i<= 50-previous_path_x.size();i++)
          {
            double N = (target_dist/(0.02 * ref_vel/2.24));  //Number of points the spline is broken into
            double x_point = x_add_on + (target_x)/N;
            double y_point = s(x_point);
          
            x_add_on = x_point;
            
            double x_ref = x_point;
            double y_ref = y_point;
            
            //Rotate back to normal after rotating it earlier
            //Transform back from local car co-ordinates to map co-ordinates
            x_point = (x_ref * cos(ref_yaw) - y_ref*sin(ref_yaw));
            y_point = (x_ref * sin(ref_yaw) + y_ref*cos(ref_yaw));
            
            x_point += ref_x;
            y_point += ref_y;
            
            next_x_vals.push_back(x_point);
            next_y_vals.push_back(y_point);
          }
          
          
          msgJson["next_x"] = next_x_vals;
          msgJson["next_y"] = next_y_vals;

          auto msg = "42[\"control\","+ msgJson.dump()+"]";

          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}
