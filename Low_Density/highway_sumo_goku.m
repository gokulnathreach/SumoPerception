function highway_sumo_goku(densidad,seed)

GUI = 0;

% R es la tasa de generaci�n de veh�culos (R veh�culos por segundo)
  
    % Par�metros de configuracion:
    carpeta_sumo = '"/home/goku16/ns3/SUMO/sumo-git/bin/"';
    Hz = 10; % CAM transmitidos por segundo por cada vehiculo
    t_ini = 0;      
    t_fin = 1000; % 
    t_inc = 1/Hz;    
    t_tran = 0;
    speed_max = 140; % km/h 
    n_lanes = 3; % numero de carriles en cada sentido
    n_directions = 2;
    segment_length = 200; % length of initial and final segments in meters
    highway_length = 5000 - 2*segment_length; % length of the main road in meters
    
    seed = 1;
    densidad = 9;
    
    RandStream.setGlobalStream(RandStream('mt19937ar','seed', seed ));
  
    speedm_max = speed_max*1000/3600; %To convert m/s
    
    R = densidad*speedm_max*n_lanes*n_directions/1000;
    
    dens_total=round(densidad*n_lanes*n_directions);
         
    nombre =  ['high-' num2str(dens_total) 'veh-Tsim' num2str(t_fin) '-lanes' num2str(2)  '-ways' num2str(n_directions)  '-seed' num2str(seed) ];    
    nombre2 =  ['high-' num2str(dens_total) 'veh-Tsim' num2str(t_fin) '-lanes' num2str(2)  '-ways' num2str(n_directions)];     
    
    % Level of Service criteria for basic freeway segments (Exhibit 23-2 Highway Capacity Manuel 2000, page 869)
    % Levels F and G from http://www.uwicore.umh.es/files/paper/2013_international/CoTEC_Bauza_Gozalvez_uwicore.pdf
    LOS.density = [7 11 16 22 28 37 50]; % cars/km/lane
    LOS.speed   = [120 120 114.6 99.6 85.7 64.5 44]; % km/h
    LOS.name = ['A' ; 'B' ; 'C'; 'D';  'E'; 'F' ; 'G'];
  
% 0.- Creamos la autovia:
% http://sumo.dlr.de/wiki/Networks/Building_Networks_from_own_XML-descriptions 

    disp('-- Creating the nodes and lanes of the highway manually,  Creando los nodos y carriles de la autovia manualmente ...')
    tic

    nombre_nod = [nombre2 '.nod.xml'];    % nod = intersecciones
    delete(nombre_nod)
    diary(nombre_nod)   
    if segment_length > 0
%         disp('<nodes>')
%         disp(['    <node id="0" x="0" y="0.0" type="priority"/> '])
%         disp(['    <node id="1" x="' num2str(segment_length) '" y="0.0" type="priority"/> '])
%         disp(['    <node id="2" x="' num2str(segment_length + highway_length/2) '" y="0.0" type="priority"/> '])
%         disp(['    <node id="3" x="' num2str(segment_length + highway_length) '" y="0.0" type="priority"/> '])
%         disp(['    <node id="4" x="' num2str(2*segment_length + highway_length) '" y="0.0" type="priority"/> '])
%         disp('</nodes> ')    
        disp('<nodes>')
        disp(['    <node id="0" x="0" y="10.0" type="priority"/> '])
        disp(['    <node id="1" x="' num2str(segment_length) '" y="10.0" type="priority"/> '])
        disp(['    <node id="2" x="' num2str(segment_length + highway_length) '" y="10.0" type="priority"/> '])        
        disp(['    <node id="3" x="' num2str(2*segment_length + highway_length) '" y="10.0" type="priority"/> '])
        disp('</nodes> ')
        nnodes=3;
    else
        disp('<nodes>')
        disp(['    <node id="0" x="0" y="10.0" type="priority"/> '])
        disp(['    <node id="1" x="' num2str(highway_length) '" y="10.0" type="priority"/> '])
        disp('</nodes> ')
        nnodes = 1;
    end
    diary off
    
    nombre_edg = [nombre2 '.edg.xml'];    % edge = calles
    delete(nombre_edg)
    diary(nombre_edg)    
    disp(['<edges>'])
    for k=0:(nnodes-1)
        disp(['   <edge id="' num2str(k) '_' num2str(k+1) '" from="' num2str(k) '" to="' num2str(k+1) '" numLanes="' num2str(n_lanes) '" speed="' num2str(speedm_max) '"/>'])
        if n_directions==2
            disp(['   <edge id="' num2str(k+1) '_' num2str(k) '" from="' num2str(k+1) '" to="' num2str(k) '" numLanes="' num2str(n_lanes) '" speed="' num2str(speedm_max) '"/>'])
        end
    end
    disp(['</edges>'])    
    diary off  
    
    toc    
        
% 1.- Crear el archivo de red con NETCONVERT

    disp('-- Crear el archivo de red con NETCONVERT ...')
    tic
    nombre_net = [nombre2 '.net.xml'];
    netconvert = ['/home/goku16/ns3/SUMO/sumo-git/bin/netconvert'];      
    dos([netconvert ' --no-turnarounds=true --node-files=' nombre_nod ' --edge-files=' nombre_edg ' --output-file=' nombre_net])          
    toc
    
% 2.- Generacion de rutas aleatorias por el manhattan:
    
if 0
    disp('-- Creando el trafico con randomTrips.py ...')
    tic
    nombre_rou = [nombre '.rou.xml'];     
    cadena = 'python "/home/goku16/ns3/SUMO/sumo-git/tools"';
    cadena = [cadena ' --net-file ' nombre_net ' --route-file ' nombre_rou ];
    cadena = [cadena ' -b ' num2str(t_ini) ' -e ' num2str(t_fin)];
    cadena = [cadena ' --fringe-factor 100000']; % increases the probability that trips will start/end at the fringe of the network.
    cadena = [cadena ' --trip-attributes="departLane=\"free\" departSpeed=\"max\" departPos=\"random_free\""']; % http://sumo.dlr.de/wiki/Definition_of_Vehicles,_Vehicle_Types,_and_Routes#departLane
    cadena = [cadena ' --period ' num2str(1/R) ' --seed ' num2str(seed)];    
    dos(cadena)   
    toc    
else
    disp('-- Creando flujos ...')
    tic
    nombre_rou = [nombre '.rou.xml'];        
    
%     delete(nombre_rou)
%     diary(nombre_rou)    
%     disp('<routes>')
%     disp(['<flow id="type1" color="1,1,0"  begin="' num2str(t_ini) '" end= "' num2str(t_fin) '" period="' num2str(2/R) '">'])
%     disp('    <route edges="0_1 1_2 2_3"/>')    
%     disp('</flow>')
%     disp(['<flow id="type2" color="1,1,0"  begin="' num2str(t_ini) '" end= "' num2str(t_fin) '" period="' num2str(2/R) '">'])
%     disp('    <route edges="3_2 2_1 1_0"/>')    
%     disp('</flow>')
%     disp('</routes>')    
%     diary off  
    
    delete(nombre_rou)
    diary(nombre_rou)    
    disp('<routes>')
    disp(['<route id="route0" color="1,1,0" edges="0_1 1_2 2_3"/>'])
    disp(['<route id="route1" color="1,1,0" edges="3_2 2_1 1_0"/>'])
    for ID = 0:2:(R*(t_fin-t_ini))
        disp(['<vehicle id="' num2str(ID) '" route="route0" depart="' num2str(ID/R + 0.1*rand/R) '" color="1,0,0" departLane="free" departSpeed="max" departPos="random_free"/>'])
        disp(['<vehicle id="' num2str(ID+1) '" route="route1" depart="' num2str((ID+1)/R + 0.1*rand/R) '" color="1,0,0"  departLane="free" departSpeed="max" departPos="random_free"/>'])
    end    
    disp('</routes>')    
    diary off    
        
    toc  
end
    

% 3.- Generacion del archivo de configuracion:

    disp('-- Creando archivo de configuracion de sumo ...')
    tic

    nombre_dump = [nombre '.netstate.xml'];
    nombre_emi = [nombre '.emi.xml'];

    nombre_cfg = [nombre '.sumo.cfg'];
    
    delete(nombre_cfg)    
    diary(nombre_cfg)

    disp('<configuration>')
    disp('    <files>')
    disp(['        <net-file>' nombre_net '</net-file>']);        
    disp(['        <route-files>' nombre_rou '</route-files>']);    
    disp('    </files>')
    disp('    <time>')
    disp(['        <begin value="' num2str(t_ini) '"/>'])
    disp(['        <end value="' num2str(t_fin)   '"/>'])    
    disp(['        <step-length value="' num2str(t_inc)   '"/>'])
    disp('    </time>');  
    disp('    <processing>'); % http://www.sumo.dlr.de/userdoc/SUMO.html
    disp('        <time-to-teleport value="-1"/>'); % Specify how long a vehicle may wait until being teleported, defaults to 300, non-positive values disable teleporting; default: 300
    disp('    </processing>');      
    disp('</configuration>')
    
    diary off
    toc
    
    % 5.- Generacion del archivo lanzamiento para omnet:

    disp('-- Creando archivo launchd ...')
    tic

    nombre_launch = [nombre '.launchd.xml'];
    
    delete(nombre_launch)   
    
    diary(nombre_launch)    
    disp('<launch>')
    disp(['<copy file="' nombre_net '"/>'])
    disp(['<copy file="' nombre_rou '"/>'])
    disp(['<copy file="' nombre_cfg '" type="config"/>'])
    disp(['<copy file="' nombre_launch '"/>'])
    disp('</launch>')

    diary off
    toc
    
    
   

        
    
    
    



    
   
        
   
        

        
 