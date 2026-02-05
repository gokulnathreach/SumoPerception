void ForwardSensing(int sendernode, NodeContainer Nodes, int sensorno){

	//std::cout << "FORWARD SENSING :  " << "Sensor Number :  "<< sensorno << std::endl;
	 uint32_t ticket = 0;
	 uint32_t ticket1 = 0;
	 uint32_t detectedvehicles = 0;
	 uint32_t k = 0;


	 double maskangle [4];
	 double minangle ;
	 double maxangle ;

	double detectedvehiclesID [NumCars]; //goku: we can also initialize [NumCars-1]
	double maskvehiclesID [NumCars];
	double nonmaskvehiclesID [NumCars];

	//Sendernodeid details
//	      double t = (Simulator::Now ()).GetSeconds ();

		  Ptr<Node> Sender = Nodes.Get (sendernode);
		  Ptr<MobilityModel> modelx = Sender ->GetObject<MobilityModel>();
		  int sendernodeId = modelx->GetObject<Node> ()->GetId ();
		  Vector senderposition = modelx->GetPosition ();
		  double t = (Simulator::Now ()).GetSeconds ();


//--------------SENSOR  START------------------------
		  //This for loop is to find vehicles within the sensor1 FOV
		 for (uint32_t i = 0; i < Nodes.GetN(); ++i)
		 {

			  Ptr<Node> Node_i = Nodes.Get (i);
			  Ptr<MobilityModel> model2 = Node_i ->GetObject<MobilityModel>();
			  int checknodeId = model2->GetObject<Node> ()->GetId ();
			  Vector detectedposition = model2->GetPosition ();
			 // Vector detectedvel = model2->GetVelocity ();


			  if (sendernodeId!= checknodeId)
			  {
//				  if (sendernodeId == 0){
//			  std::cout << "DETECTED Vehicle ID : " << checknodeId <<" Distance X :" << detectedposition.x << " Y :"  << detectedposition.y << std::endl;}

						  double X = detectedposition.x - senderposition.x ;
						  double Y = detectedposition.y - senderposition.y ;

//						  std::cout << "X2-X1 :  "<< X << "Y2-Y1 :  "<< Y << std::endl;


				  double distance = modelx->GetDistanceFrom (model2);
				/*  if (sendernodeId == 0){
			  std::cout << "Distance from sender node : " << sendernodeId << " to detected node "  << checknodeId << " is : " << distance << std::endl;
				  }*/

				  double angle = atan2 (Y,X)*180 / PI;
				 /* if ((sendernodeId == 0)){
				  std::cout << "Checking Node :  "<< checknodeId << "   DETECTED ANGLE :  "<< angle << std::endl;}*/

                  if (angle >= 90)
                  {
                      angle = floor(angle);
                      /* if ((sendernodeId == 0)  ){
                           std::cout << "check point "   << std::endl; }*/
                  }

                  if (angle <= -90)
                  {
                      angle = ceil(angle);
                      //std::cout << "check point "   << std::endl;
                  }
                  	  double Fsensing_distance_temp= Fsensordistance[sensorno] + carlength/2;
					  if ( (distance < Fsensing_distance_temp )   && ( ( (Fsensornegativeangle[sensorno] <=angle) && (angle<=0) ) || ( (0<=angle) && (angle<=Fsensorpositiveangle[sensorno]) ) ) )
					  {
						  detectedvehiclesID [ticket] = checknodeId;
						  	ticket++;
						  	detectedvehicles += 1;

						  	//***
						  //	if((sendernodeId == 1) || (sendernodeId == 0)){
						 // 		std::cout << "DETECTED node :  "<< checknodeId << std::endl;

					  }

					  //*********************For looging vehicles in ROI****************
						 Ptr<MobilityModel> model = Nodes.Get(sendernode) ->GetObject<MobilityModel>();
						Vector position = model->GetPosition();
						if((position.x>dist1 && position.x<dist2) && (t > log_begin_time) && (sensorno ==0) ){

							  if (distance <= 1000)
							  {

								  awarenessROI objectROI;

								  objectROI.detectedVehID = checknodeId;
								  objectROI.detectedDist = distance;
								  objectROI.detectedvehposX = detectedposition.x;
								  objectROI.detectedvehposY = detectedposition.y;
								  objectROI.detectedtime = t;

								  differentROI.insert ( std::pair<uint32_t ,awarenessROI>(sendernodeId, objectROI));

							  }





						}
					  //*********************For looging vehicles in ROI****************



		 }

		 //Store ROI vehicles in Multimap

		}


		// std::cout << "TOTAL VEHICLES DETECTED before MASKING SENSOR : "<<sensorno << "  Detected-Vehicles: " << detectedvehicles  << std::endl;

		 //std::cout << "DETECTED vehicle count  :  "<< detectedvehicles << "    TICKET count  :  "<< ticket << std::endl;

		 //This for loop is to detect the masked vehicles
if ( detectedvehicles > 1){
		 for (uint32_t i = 0; i < ticket; ++i)
		 		 {

			 	 	 	 Ptr<Node> Node_ticket = Nodes.Get (detectedvehiclesID [i]);
						  Ptr<MobilityModel> model3 = Node_ticket ->GetObject<MobilityModel>();
						  int checki = model3->GetObject<Node> ()->GetId ();
						  Vector detectedi = model3->GetPosition ();
						  double distance1 = modelx->GetDistanceFrom (model3);
						//  std::cout << "DETECTED Vehicle ID :" << checki << "DISTANCE $1$ :  "<< distance1 << std::endl;
						//  std::cout << "DETECTED Vehicle X position :" << detectedi.x << std::endl;
						//  std::cout << "DETECTED Vehicle Y position :" << detectedi.y << std::endl;

						  //I assumed car length as 4m and width as 2m
						  double leftpointbackx = detectedi.x -(carlength/2) ;
						  double leftpointbacky = detectedi.y + (carwidth/2);

						//  std::cout << "DETECTED Vehicle leftpointbackx position :" << leftpointbackx << std::endl;
						//  std::cout << "DETECTED Vehicle leftpointbacky position :" << leftpointbacky << std::endl;


						  double rightpointbackx = detectedi.x -(carlength/2) ;
						  double rightpointbacky = detectedi.y - (carwidth/2);

						  double leftpointfrontx = detectedi.x +(carlength/2) ;
						  double leftpointfronty = detectedi.y + (carwidth/2);

						  double rightpointfrontx = detectedi.x +(carlength/2) ;
						  double rightpointfronty = detectedi.y - (carwidth/2);

					  double medianX = detectedi.x - senderposition.x ;
					  double medianY = detectedi.y - senderposition.y ;

						  double leftbackX = leftpointbackx - senderposition.x;
						  double leftbackY = leftpointbacky - senderposition.y;

						  double rightbackX = rightpointbackx - senderposition.x;
						  double rightbackY = rightpointbacky - senderposition.y;

						  double leftfrontX = leftpointfrontx - senderposition.x;
						  double leftfrontY = leftpointfronty - senderposition.y;

						  double rightfrontX = rightpointfrontx - senderposition.x;
						  double rightfrontY = rightpointfronty - senderposition.y;

					  double maskanglemedian = atan2 (medianY,medianX)*180 / PI;
					 // if((sendernodeId == 1) || (sendernodeId == 0)){
					//  std::cout << "Median MASKING angle : " << maskanglemedian  << std::endl;

//						  std::cout << "VEHICLE BACK ANGLE "  << std::endl;

						  double maskangleleftback = atan2 (leftbackY,leftbackX)*180 / PI;
						  maskangle [0] = maskangleleftback;
						  	//***
						  	//if((sendernodeId == 1) || (sendernodeId == 0)){
						//  std::cout << "LEFT MASKING angle : " << maskangleleftback  << std::endl;



						  double maskanglerightback = atan2 (rightbackY,rightbackX)*180 / PI;
						  maskangle [1] = maskanglerightback;
					// if((sendernodeId == 1) || (sendernodeId == 0)){
					//	 std::cout << "RIGHT MASKING angle : " << maskanglerightback  << std::endl;

//					  std::cout << "VEHICLE FRONT ANGLE "  << std::endl;

						  double maskangleleftfront = atan2 (leftfrontY,leftfrontX)*180 / PI;
						  maskangle [2] = maskangleleftfront;
					// if((sendernodeId == 1) || (sendernodeId == 0)){
					//	  std::cout << "LEFT MASKING angle : " << maskangleleftback  << std::endl;

						  double maskanglerightfront = atan2 (rightfrontY,rightfrontX)*180 / PI;
						  maskangle [3] = maskanglerightfront;
					 // if((sendernodeId == 1) || (sendernodeId == 0)){
						//  std::cout << "RIGHT MASKING angle : " << maskanglerightback  << std::endl;


						  //Find the min and max angle
						  minangle = maskangle [0];
						  maxangle = maskangle [0];

						  for (uint32_t x = 0; x <= 3; x++)
						      {
						        if (maskangle[x] > maxangle)
						          {
						        	maxangle = maskangle[x];
						          }
						        else if (maskangle[x] < minangle)
						          {
						        	minangle = maskangle[x];
						          }
						      }

						 // std::cout << "CHECKING VEHICLE : " << checki << " MINIMUM MASKING angle : " << minangle  << " MAXIMUM MASKING angle : " << maxangle << std::endl;

						  for (uint32_t j = 0; j < ticket; ++j)
						  	 {
							  uint32_t alreadyexist = 0;

							  Ptr<Node> Node_ticket = Nodes.Get (detectedvehiclesID [j]);
							  Ptr<MobilityModel> model4 = Node_ticket ->GetObject<MobilityModel>();
							  int checkj = model4->GetObject<Node> ()->GetId ();
							  Vector detectedj = model4->GetPosition ();
							  double distance2 = modelx->GetDistanceFrom (model4);

							  for(uint32_t ck=0; ck < ticket1 ; ++ck)
							  {
								  if (maskvehiclesID [ck] == checkj)
								  {
									  alreadyexist = 1;
									  break;
								  }
							  }


							if( (distance1 < distance2) && (checki != checkj) && (alreadyexist ==0 ) )
							  {

//								  std::cout << "DETECTED Vehicle ID :" << checki << " DISTANCE $1$ :  "<< distance1 << " TARGET Vehicle ID :" << checkj << " DISTANCE $2$ :  "<< distance2 << std::endl;

								  double targetX = detectedj.x - senderposition.x ;
								  double targetY = detectedj.y - senderposition.y ;
								  double targetmedianangle = atan2 (targetY,targetX)*180 / PI;


								  double leftpointbackx = detectedj.x -(carlength/2) ;
								  double leftpointbacky = detectedj.y + (carwidth/2);

								  double rightpointbackx = detectedj.x -(carlength/2) ;
								  double rightpointbacky = detectedj.y - (carwidth/2);

								  double leftpointfrontx = detectedj.x +(carlength/2) ;
								  double leftpointfronty = detectedj.y + (carwidth/2);

								  double rightpointfrontx = detectedj.x +(carlength/2) ;
								  double rightpointfronty = detectedj.y - (carwidth/2);

								  double leftbackX = leftpointbackx - senderposition.x;
								  double leftbackY = leftpointbacky - senderposition.y;

								  double rightbackX = rightpointbackx - senderposition.x;
								  double rightbackY = rightpointbacky - senderposition.y;

								  double leftfrontX = leftpointfrontx - senderposition.x;
								  double leftfrontY = leftpointfronty - senderposition.y;

								  double rightfrontX = rightpointfrontx - senderposition.x;
								  double rightfrontY = rightpointfronty - senderposition.y;

								  double targetleftback = atan2 (leftbackY,leftbackX)*180 / PI;
								  double targetrightback = atan2 (rightbackY,rightbackX)*180 / PI;
								  double targetleftfront = atan2 (leftfrontY,leftfrontX)*180 / PI;
								  double targetrightfront = atan2 (rightfrontY,rightfrontX)*180 / PI;

								  /*if (checkj == ){
								  std::cout << "TARGET MEDIAN angle : " << targetmedianangle  << std::endl;
								  std::cout << "TARGET LEFT BACK angle : " << targetleftback  << std::endl;
								  std::cout << "TARGET LEFT FRONT angle : " << targetleftfront  << std::endl;
								  std::cout << "TARGET RIGHT BACK angle : " << targetrightback  << std::endl;
								  std::cout << "TARGET RIGHT FRONT angle : " << targetrightfront  << std::endl;
								  }*/

								//	if ( ((targetangle <= maxangle) && (targetangle >= minangle)) )
								  if ( ((targetmedianangle <= maxangle) && (targetmedianangle >= minangle))  ||
								       (    ( ((targetleftback <= maxangle) && (targetleftfront <= maxangle) ) && ( (targetleftback  >= minangle) && (targetleftfront  >= minangle) ) ) ||
								            ( ((targetrightback <= maxangle) && (targetrightfront <= maxangle)) && ((targetrightback  >= minangle) && (targetrightfront >=minangle) ) )     )  || ( targetmedianangle == maskanglemedian)   )
											 {
											   maskvehiclesID [ticket1] = checkj;
											   ticket1++;
											 }
							   }

					 }

		 		 }
		}

		 //Now we have two list  detectedvehiclesID[] list and maskvehiclesID[] list.
		 //Compare both list and find the vehicles that are not masked

		     detectedvehicles = 0;
		 			for (uint32_t i = 0; i < ticket; ++i)
		 			 {
		 			     int find =0;
		 				 for (uint32_t j = 0; j < ticket1; ++j)
		 				 {
		 					if( detectedvehiclesID[i] == maskvehiclesID[j])
		 					{
		 					 find += 1;
		 					break;
		 					}
		 				 }
		 				if ( find == 0)
		 				{
		 					nonmaskvehiclesID[k] = detectedvehiclesID[i];
		 				    k += 1;
		 				   detectedvehicles += 1;

		 				}
		 			 }

		  Globalflag =  Globalflag + k;

		 //  std::cout << "TOTAL VEHICLES DETECTED *AFTER* SENSOR " << sensorno <<" MASKING : " << detectedvehicles << std::endl;


		   			  for(uint32_t i = 0; tvcount < Globalflag ; ++tvcount , ++i  )
		   			  {Totaldetectedvehicles[tvcount] = nonmaskvehiclesID[i]; }

//-------------- SENSOR ENDS---------------

}


void ReverseSensing(int sendernode, NodeContainer Nodes, int sensorno){

	//std::cout << "REVERSE SENSING  :  " << std::endl;
	 uint32_t ticket = 0;
	 uint32_t ticket1 = 0;
	 uint32_t detectedvehicles = 0;
	 uint32_t k = 0;

	 //For ROI
//	 uint32_t vehiclesin100 = 0;
//	 uint32_t vehiclesin200 = 0;
//	 uint32_t vehiclesin300 = 0;
//	 uint32_t vehiclesin400 = 0;
//	 uint32_t vehiclesin500 = 0;

	 double maskangle [4];
	 double minangle ;
	 double maxangle ;

	double detectedvehiclesID [NumCars]; //goku: we can also initialize [NumCars-1]
	double maskvehiclesID [NumCars];
	double nonmaskvehiclesID [NumCars];

	//Sendernodeid details
//	      double t = (Simulator::Now ()).GetSeconds ();

		  Ptr<Node> Sender = Nodes.Get (sendernode);
		  Ptr<MobilityModel> modelx = Sender ->GetObject<MobilityModel>();
		  int sendernodeId = modelx->GetObject<Node> ()->GetId ();
		  Vector senderposition = modelx->GetPosition ();
		  double t = (Simulator::Now ()).GetSeconds ();


//--------------SENSOR  START------------------------
		  //This for loop is to find vehicles within the sensor1 FOV
		 for (uint32_t i = 0; i < Nodes.GetN(); ++i)
		 {
			 		
			  Ptr<Node> Node_i = Nodes.Get (i);
			  Ptr<MobilityModel> model2 = Node_i ->GetObject<MobilityModel>();
			  int checknodeId = model2->GetObject<Node> ()->GetId ();
			  Vector detectedposition = model2->GetPosition ();
			 // Vector detectedvel = model2->GetVelocity ();


			  if (sendernodeId!= checknodeId)
			  {
//				  if (sendernodeId == 0){
//			  std::cout << "DETECTED Vehicle ID : " << checknodeId <<" Distance X :" << detectedposition.x << " Y :"  << detectedposition.y << std::endl;}

						  double X = detectedposition.x - senderposition.x ;
						  double Y = detectedposition.y - senderposition.y ;

//						  std::cout << "X2-X1 :  "<< X << "Y2-Y1 :  "<< Y << std::endl;


				  double distance = modelx->GetDistanceFrom (model2);
				/*  if (sendernodeId == 0){
			  std::cout << "Distance from sender node : " << sendernodeId << " to detected node "  << checknodeId << " is : " << distance << std::endl;
			  }*/

				  double angle = atan2 (Y,X)*180 / PI;
				/* if ((sendernodeId == 9)  ){
				 std::cout << "DETECTED Node :  "<< checknodeId << "   DETECTED ANGLE :  "<< angle << std::endl; }*/
					// std::cout << "DETECTED Node :  "<< checknodeId << "   DETECTED ANGLE :  "<< angle << std::endl;

                  if (angle >= 180)
                  {
                      angle = floor(angle);
                      /* if ((sendernodeId == 0)  ){
                           std::cout << "check point "   << std::endl; }*/
                  }

                  if (angle <= -180)
                  {
                      angle = ceil(angle);
                      //std::cout << "check point "   << std::endl;
                  }

             	  double Rsensing_distance_temp= Rsensordistance[sensorno] + carlength/2;
				 if ( (distance < Rsensing_distance_temp)  && ( ( (angle <= Rsensornegativeangle[sensorno] ) && (angle >= -180) ) || ( (angle >= Rsensorpositiveangle[sensorno] ) && (angle<= 180) ) ) )
					  {
						  detectedvehiclesID [ticket] = checknodeId;
						  	ticket++;
						  	detectedvehicles += 1;

						  	//***
						  //	if((sendernodeId == 1) || (sendernodeId == 0)){
						 // 		std::cout << "DETECTED node :  "<< checknodeId << std::endl;

					  }

					  //*********************For looging vehicles in ROI****************
				 Ptr<MobilityModel> model = Nodes.Get(sendernode) ->GetObject<MobilityModel>();
				Vector position = model->GetPosition();
				if((position.x>dist1 && position.x<dist2) && (t > log_begin_time) && (sensorno ==0) ){

					  if (distance <= 1000)
					  {

						  awarenessROI objectROI;

						  objectROI.detectedVehID = checknodeId;
						  objectROI.detectedDist = distance;
						  objectROI.detectedvehposX = detectedposition.x;
						  objectROI.detectedvehposY = detectedposition.y;
						  objectROI.detectedtime = t;

						  differentROI.insert ( std::pair<uint32_t ,awarenessROI>(sendernodeId, objectROI));

					  }





				}

					  //*********************For looging vehicles in ROI****************


			  }
		 }

		 //std::cout << "TOTAL VEHICLES DETECTED before MASKING SENSOR : "<<sensorno << "  Detected-Vehicles: " << detectedvehicles  << std::endl;

		//std::cout << "DETECTED vehicle count  :  "<< detectedvehicles  << std::endl;

		 //This for loop is to detect the masked vehicles
if ( detectedvehicles > 1){
		 for (uint32_t i = 0; i < ticket; ++i)
		 		 {

			 	 	 	 Ptr<Node> Node_ticket = Nodes.Get (detectedvehiclesID [i]);
						  Ptr<MobilityModel> model3 = Node_ticket ->GetObject<MobilityModel>();
						  int checki = model3->GetObject<Node> ()->GetId ();
						  Vector detectedi = model3->GetPosition ();
						  double distance1 = modelx->GetDistanceFrom (model3);
						//  std::cout << "DETECTED Vehicle ID :" << checki << "DISTANCE $1$ :  "<< distance1 << std::endl;
						//  std::cout << "DETECTED Vehicle X position :" << detectedi.x << std::endl;
						//  std::cout << "DETECTED Vehicle Y position :" << detectedi.y << std::endl;

						  //I assumed car length as 4m and width as 2m
						  double leftpointbackx = detectedi.x -(carlength/2) ;
						  double leftpointbacky = detectedi.y + (carwidth/2);

						//  std::cout << "DETECTED Vehicle leftpointbackx position :" << leftpointbackx << std::endl;
						//  std::cout << "DETECTED Vehicle leftpointbacky position :" << leftpointbacky << std::endl;


						  double rightpointbackx = detectedi.x -(carlength/2) ;
						  double rightpointbacky = detectedi.y - (carwidth/2);

						  double leftpointfrontx = detectedi.x +(carlength/2) ;
						  double leftpointfronty = detectedi.y + (carwidth/2);

						  double rightpointfrontx = detectedi.x +(carlength/2) ;
						  double rightpointfronty = detectedi.y - (carwidth/2);

					  double medianX = detectedi.x - senderposition.x ;
					  double medianY = detectedi.y - senderposition.y ;

						  double leftbackX = leftpointbackx - senderposition.x;
						  double leftbackY = leftpointbacky - senderposition.y;

						  double rightbackX = rightpointbackx - senderposition.x;
						  double rightbackY = rightpointbacky - senderposition.y;

						  double leftfrontX = leftpointfrontx - senderposition.x;
						  double leftfrontY = leftpointfronty - senderposition.y;

						  double rightfrontX = rightpointfrontx - senderposition.x;
						  double rightfrontY = rightpointfronty - senderposition.y;

					  double maskanglemedian = atan2 (medianY,medianX)*180 / PI;
					 // if((sendernodeId == 1) || (sendernodeId == 0)){
					//  std::cout << "Median MASKING angle : " << maskanglemedian  << std::endl;

//						  std::cout << "VEHICLE BACK ANGLE "  << std::endl;

						  double maskangleleftback = atan2 (leftbackY,leftbackX)*180 / PI;
						  maskangle [0] = maskangleleftback;
						  if(maskangleleftback < 0)
						  {
							  maskangle [0] = maskangleleftback + 360;
						  }

						  	//***
						  	//if((sendernodeId == 1) || (sendernodeId == 0)){
						//  std::cout << "LEFT MASKING angle : " << maskangleleftback  << std::endl;



						  double maskanglerightback = atan2 (rightbackY,rightbackX)*180 / PI;
						  maskangle [1] = maskanglerightback;
						  if(maskanglerightback < 0)
						  {
							  maskangle [1] = maskanglerightback + 360;
						  }

					// if((sendernodeId == 1) || (sendernodeId == 0)){
					//	 std::cout << "RIGHT MASKING angle : " << maskanglerightback  << std::endl;

//					  std::cout << "VEHICLE FRONT ANGLE "  << std::endl;

						  double maskangleleftfront = atan2 (leftfrontY,leftfrontX)*180 / PI;
						  maskangle [2] = maskangleleftfront;
						  if(maskangleleftfront < 0)
						  {
							  maskangle [2] = maskangleleftfront + 360;
						  }

					// if((sendernodeId == 1) || (sendernodeId == 0)){
					//	  std::cout << "LEFT MASKING angle : " << maskangleleftback  << std::endl;

						  double maskanglerightfront = atan2 (rightfrontY,rightfrontX)*180 / PI;
						  maskangle [3] = maskanglerightfront;
						  if(maskanglerightfront < 0)
						  {
							  maskangle [3] = maskanglerightfront + 360;
						  }

					 // if((sendernodeId == 1) || (sendernodeId == 0)){
						//  std::cout << "RIGHT MASKING angle : " << maskanglerightback  << std::endl;


						  //Find the min and max angle
						  minangle = maskangle [0];
						  maxangle = maskangle [0];

						  for (uint32_t x = 0; x <= 3; x++)
						      {
						        if (maskangle[x] > maxangle)
						          {
						        	maxangle = maskangle[x];
						          }
						        else if (maskangle[x] < minangle)
						          {
						        	minangle = maskangle[x];
						          }
						      }

						 // if((sendernodeId == 0)){
						// std::cout << "CHECKING VEHICLE : " << checki << " MINIMUM MASKING angle : " << minangle  << " MAXIMUM MASKING angle : " << maxangle << std::endl;}

						  for (uint32_t j = 0; j < ticket; ++j)
						  	 {
							  uint32_t alreadyexist = 0;

							  Ptr<Node> Node_ticket = Nodes.Get (detectedvehiclesID [j]);
							  Ptr<MobilityModel> model4 = Node_ticket ->GetObject<MobilityModel>();
							  int checkj = model4->GetObject<Node> ()->GetId ();
							  Vector detectedj = model4->GetPosition ();
							  double distance2 = modelx->GetDistanceFrom (model4);

							  for(uint32_t ck=0; ck < ticket1 ; ++ck)
							  {
								  if (maskvehiclesID [ck] == checkj)
								  {
									  alreadyexist = 1;
									  break;
								  }
							  }


							if( (distance1 < distance2) && (checki != checkj) && (alreadyexist ==0 ) )
							  {
							//	 if((sendernodeId == 0)){
							//	 std::cout << "DETECTED Vehicle ID :" << checki << " DISTANCE $1$ :  "<< distance1 << " TARGET Vehicle ID :" << checkj << " DISTANCE $2$ :  "<< distance2 << std::endl; }

								  double targetX = detectedj.x - senderposition.x ;
								  double targetY = detectedj.y - senderposition.y ;
								  double targetmedianangle = atan2 (targetY,targetX)*180 / PI;
								  if(targetmedianangle < 0)
								  {
									  targetmedianangle = targetmedianangle + 360;
								  }

								  double leftpointbackx = detectedj.x -(carlength/2) ;
								  double leftpointbacky = detectedj.y + (carwidth/2);

								  double rightpointbackx = detectedj.x -(carlength/2) ;
								  double rightpointbacky = detectedj.y - (carwidth/2);

								  double leftpointfrontx = detectedj.x +(carlength/2) ;
								  double leftpointfronty = detectedj.y + (carwidth/2);

								  double rightpointfrontx = detectedj.x +(carlength/2) ;
								  double rightpointfronty = detectedj.y - (carwidth/2);

								  double leftbackX = leftpointbackx - senderposition.x;
								  double leftbackY = leftpointbacky - senderposition.y;

								  double rightbackX = rightpointbackx - senderposition.x;
								  double rightbackY = rightpointbacky - senderposition.y;

								  double leftfrontX = leftpointfrontx - senderposition.x;
								  double leftfrontY = leftpointfronty - senderposition.y;

								  double rightfrontX = rightpointfrontx - senderposition.x;
								  double rightfrontY = rightpointfronty - senderposition.y;

								  double targetleftback = atan2 (leftbackY,leftbackX)*180 / PI;
								  double targetrightback = atan2 (rightbackY,rightbackX)*180 / PI;
								  double targetleftfront = atan2 (leftfrontY,leftfrontX)*180 / PI;
								  double targetrightfront = atan2 (rightfrontY,rightfrontX)*180 / PI;

								  if(targetleftback < 0)
								  {
									  targetleftback = targetleftback + 360;
								  }

								  if(targetrightback < 0)
								  {
									  targetrightback = targetrightback + 360;
								  }

								  if(targetleftfront < 0)
								  {
									  targetleftfront = targetleftfront + 360;
								  }

								  if(targetrightfront < 0)
								  {
									  targetrightfront = targetrightfront + 360;
								  }


							/*	 std::cout << "TARGET MEDIAN angle : " << targetmedianangle  << std::endl;
								  std::cout << "TARGET LEFT BACK angle : " << targetleftback  << std::endl;
								  std::cout << "TARGET LEFT FRONT angle : " << targetleftfront  << std::endl;
								  std::cout << "TARGET RIGHT BACK angle : " << targetrightback  << std::endl;
								  std::cout << "TARGET RIGHT FRONT angle : " << targetrightfront  << std::endl;*/


								//	if ( ((targetangle <= maxangle) && (targetangle >= minangle)) )
								  if ( ((targetmedianangle <= maxangle) && (targetmedianangle >= minangle))  ||
								       (    ( ((targetleftback <= maxangle) && (targetleftfront <= maxangle) ) && ( (targetleftback  >= minangle) && (targetleftfront  >= minangle) ) ) ||
								            ( ((targetrightback <= maxangle) && (targetrightfront <= maxangle)) && ((targetrightback  >= minangle) && (targetrightfront >=minangle) ) )     )  || (targetmedianangle == maskanglemedian  )    )
											 {
											   maskvehiclesID [ticket1] = checkj;
											   ticket1++;
											 }
							   }

					 }

		 		 }
		}

		 //Now we have two list  detectedvehiclesID[] list and maskvehiclesID[] list.
		 //Compare both list and find the vehicles that are not masked

		     detectedvehicles = 0;
		 			for (uint32_t i = 0; i < ticket; ++i)
		 			 {
		 			     int find =0;
		 				 for (uint32_t j = 0; j < ticket1; ++j)
		 				 {
		 					if( detectedvehiclesID[i] == maskvehiclesID[j])
		 					{
		 					 find += 1;
		 					break;
		 					}
		 				 }
		 				if ( find == 0)
		 				{
		 					nonmaskvehiclesID[k] = detectedvehiclesID[i];
		 				    k += 1;
		 				   detectedvehicles += 1;

		 				}
		 			 }

		  Globalflag =  Globalflag + k;

		   //std::cout << "TOTAL VEHICLES DETECTED *AFTER* SENSOR " << sensorno <<" MASKING : " << detectedvehicles << std::endl;



		   			  for(uint32_t i = 0; tvcount < Globalflag ; ++tvcount , ++i  )
		   			  {Totaldetectedvehicles[tvcount] = nonmaskvehiclesID[i];
		   			 }

//-------------- SENSOR ENDS---------------

}