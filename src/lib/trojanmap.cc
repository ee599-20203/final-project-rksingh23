#include "trojanmap.h"

#include <cmath>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iomanip> 
#include <cfloat>

#include <algorithm>
#include <fstream>
#include <locale>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <utility>
#include <functional>
#include <queue>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

//-----------------------------------------------------
// TODO (Students): You do not and should not change the following functions:
//-----------------------------------------------------

/**
 * PrintMenu: Create the menu
 * 
 */
void TrojanMap::PrintMenu() {

  std::string menu =
      "**************************************************************\n"
      "* Select the function you want to execute.                    \n"
      "* 1. Autocomplete                                             \n"
      "* 2. Find the position                                        \n"
      "* 3. CalculateShortestPath                                    \n"
      "* 4. Travelling salesman problem                              \n"
      "* 5. Exit                                                     \n"
      "**************************************************************\n";
  std::cout << menu << std::endl;
  std::string input;
  getline(std::cin, input);
  char number = input[0];
  switch (number)
  {
  case '1':
  {
    menu =
        "**************************************************************\n"
        "* 1. Autocomplete                                             \n"
        "**************************************************************\n";
    std::cout << menu << std::endl;
    menu = "Please input a partial location:";
    std::cout << menu;
    getline(std::cin, input);
    auto results = Autocomplete(input);
    menu = "*************************Results******************************\n";
    std::cout << menu;
    if (results.size() != 0) {
      for (auto x : results) std::cout << x << std::endl;
    } else {
      std::cout << "No matched locations." << std::endl;
    }
    menu = "**************************************************************\n";
    std::cout << menu << std::endl;
    PrintMenu();
    break;
  }
  case '2':
  {
    menu =
        "**************************************************************\n"
        "* 2. Find the position                                        \n"
        "**************************************************************\n";
    std::cout << menu << std::endl;
    menu = "Please input a location:";
    std::cout << menu;
    getline(std::cin, input);
    auto results = GetPosition(input);
    menu = "*************************Results******************************\n";
    std::cout << menu;
    if (results.first != -1) {
      std::cout << "Latitude: " << std::setprecision(10)<< results.first
                << " Longitude: " << std::setprecision(10)<< results.second << std::endl;
      PlotPoint(results.first, results.second);
    } else {
      std::cout << "No matched locations." << std::endl;
    }
    menu = "**************************************************************\n";
    std::cout << menu << std::endl;
    PrintMenu();
    break;
  }
  case '3':
  {
    menu =
        "**************************************************************\n"
        "* 3. CalculateShortestPath                                            "
        "      \n"
        "**************************************************************\n";
    std::cout << menu << std::endl;
    menu = "Please input the start location:";
    std::cout << menu;
    std::string input1;
    getline(std::cin, input1);
    menu = "Please input the destination:";
    std::cout << menu;
    std::string input2;
    getline(std::cin, input2);
    auto results = CalculateShortestPath(input1, input2);
    menu = "*************************Results******************************\n";
    std::cout << menu;
    if (results.size() != 0) {
      for (auto x : results) std::cout << x << std::endl;
      PlotPath(results);
    } else {
      std::cout << "No route from the start point to the destination."
                << std::endl;
    }
    menu = "**************************************************************\n";
    std::cout << menu << std::endl;
    PrintMenu();
    break;
  }
  case '4':
  {
    menu =
        "**************************************************************\n"
        "* 4. Travelling salesman problem                              \n"
        "**************************************************************\n";
    std::cout << menu << std::endl;
    menu = "In this task, we will select N random points on the map and you need to find the path to travel these points and back to the start point.";
    std::cout << menu << std::endl << std::endl;
    menu = "Please input the number of the places:";
    std::cout << menu;
    getline(std::cin, input);
    int num = std::stoi(input);
    std::vector<std::string> keys;
    for (auto x : data) {
      keys.push_back(x.first);
    }
    std::vector<std::string> locations;
    srand(time(NULL));
    for (int i = 0; i < num; i++)
      locations.push_back(keys[rand() % keys.size()]);
    PlotPoints(locations);
    std::cout << "Calculating ..." << std::endl;
    auto results = TravellingTrojan(locations);
    menu = "*************************Results******************************\n";
    std::cout << menu;
    CreateAnimation(results.second);
    if (results.second.size() != 0) {
      for (auto x : results.second[results.second.size()-1]) std::cout << x << std::endl;
      menu = "**************************************************************\n";
      std::cout << menu;
      std::cout << "The distance of the path is:" << results.first << std::endl;
      PlotPath(results.second[results.second.size()-1]);
    } else {
      std::cout << "The size of the path is 0" << std::endl;
    }
    menu = "**************************************************************\n"
           "You could find your animation at src/lib/output.avi.          \n";
    std::cout << menu << std::endl;
    PrintMenu();
    break;
  }
  case '5':
    break;
  default:
    std::cout << "Please select 1 - 5." << std::endl;
    PrintMenu();
    break;
  }
}


/**
 * CreateGraphFromCSVFile: Read the map data from the csv file
 * 
 */
void TrojanMap::CreateGraphFromCSVFile() {
  std::fstream fin;
  fin.open("src/lib/map.csv", std::ios::in);
  std::string line, word;

  getline(fin, line);
  while (getline(fin, line)) {
    std::stringstream s(line);

    Node n;
    int count = 0;
    while (getline(s, word, ',')) {
      word.erase(std::remove(word.begin(), word.end(), '\''), word.end());
      word.erase(std::remove(word.begin(), word.end(), '"'), word.end());
      word.erase(std::remove(word.begin(), word.end(), '['), word.end());
      word.erase(std::remove(word.begin(), word.end(), ']'), word.end());
      if (count == 0)      //collumn 0
        n.id = word;
      else if (count == 1) //collumn 1
        n.lat = stod(word);
      else if (count == 2) //column 2
        n.lon = stod(word);
      else if (count == 3) //collumn 3
        {
          n.name = word;
          if(!word.empty())    
          {
            nameVector.push_back(word);
            std::pair<double,double> pair1=std::make_pair(n.lat,n.lon);
            location_map.insert (std::pair<std::string, std::pair<double, double>> (word,pair1));  
          }
        }
      else                //collumn 4
      word.erase(std::remove(word.begin(), word.end(), ' '), word.end());
        n.neighbors.push_back(word);
      count++;
    }
    data[n.id] = n;
  }
  fin.close();
}

/**
 * PlotPoint: Given a location id, plot the point on the map
 * 
 * @param  {std::string} id : location id
 */
void TrojanMap::PlotPoint(std::string id) {
  std::string image_path = cv::samples::findFile("src/lib/input.jpg");
  cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
  auto result = GetPlotLocation(data[id].lat, data[id].lon);
  cv::circle(img, cv::Point(result.first, result.second), DOT_SIZE,
             cv::Scalar(0, 0, 255), cv::FILLED);
  cv::imshow("TrojanMap", img);
  cv::waitKey(1);
}
/**
 * PlotPoint: Given a lat and a lon, plot the point on the map
 * 
 * @param  {double} lat : latitude
 * @param  {double} lon : longitude
 */
void TrojanMap::PlotPoint(double lat, double lon) {
  std::string image_path = cv::samples::findFile("src/lib/input.jpg");
  cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
  auto result = GetPlotLocation(lat, lon);
  cv::circle(img, cv::Point(int(result.first), int(result.second)), DOT_SIZE,
             cv::Scalar(0, 0, 255), cv::FILLED);
  cv::startWindowThread();
  cv::imshow("TrojanMap", img);
  cv::waitKey(1);
}

/**
 * PlotPath: Given a vector of location ids draws the path (connects the points)
 * 
 * @param  {std::vector<std::string>} location_ids : path
 */
void TrojanMap::PlotPath(std::vector<std::string> &location_ids) {
  std::string image_path = cv::samples::findFile("src/lib/input.jpg");
  cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
  auto start = GetPlotLocation(data[location_ids[0]].lat, data[location_ids[0]].lon);
  cv::circle(img, cv::Point(int(start.first), int(start.second)), DOT_SIZE,
             cv::Scalar(0, 0, 255), cv::FILLED);
  for (auto i = 1; i < location_ids.size(); i++) {
    auto start = GetPlotLocation(data[location_ids[i - 1]].lat, data[location_ids[i - 1]].lon);
    auto end = GetPlotLocation(data[location_ids[i]].lat, data[location_ids[i]].lon);
    cv::circle(img, cv::Point(int(end.first), int(end.second)), DOT_SIZE,
               cv::Scalar(0, 0, 255), cv::FILLED);
    cv::line(img, cv::Point(int(start.first), int(start.second)),
             cv::Point(int(end.first), int(end.second)), cv::Scalar(0, 255, 0),
             LINE_WIDTH);
  }
  cv::startWindowThread();
  cv::imshow("TrojanMap", img);
  cv::waitKey(1);
}

/**
 * PlotPoints: Given a vector of location ids draws the points on the map (no path).
 * 
 * @param  {std::vector<std::string>} location_ids : points
 */
void TrojanMap::PlotPoints(std::vector<std::string> &location_ids) {
  std::string image_path = cv::samples::findFile("src/lib/input.jpg");
  cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
  for (auto x : location_ids) {
    auto result = GetPlotLocation(data[x].lat, data[x].lon);
    cv::circle(img, cv::Point(result.first, result.second), DOT_SIZE,
               cv::Scalar(0, 0, 255), cv::FILLED);
  }
  cv::imshow("TrojanMap", img);
  cv::waitKey(1);
}


/**
 * CreateAnimation: Create the videos of the progress to get the path
 * 
 * @param  {std::vector<std::vector<std::string>>} path_progress : the progress to get the path
 */
void TrojanMap::CreateAnimation(std::vector<std::vector<std::string>> path_progress){
  cv::VideoWriter video("src/lib/output.avi", cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(1248,992));
  for(auto location_ids: path_progress) {
    std::string image_path = cv::samples::findFile("src/lib/input.jpg");
    cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
    auto start = GetPlotLocation(data[location_ids[0]].lat, data[location_ids[0]].lon);
    cv::circle(img, cv::Point(int(start.first), int(start.second)), DOT_SIZE,
              cv::Scalar(0, 0, 255), cv::FILLED);
    for (auto i = 1; i < location_ids.size(); i++) {
      auto start = GetPlotLocation(data[location_ids[i - 1]].lat, data[location_ids[i - 1]].lon);
      auto end = GetPlotLocation(data[location_ids[i]].lat, data[location_ids[i]].lon);
      cv::circle(img, cv::Point(int(end.first), int(end.second)), DOT_SIZE,
                cv::Scalar(0, 0, 255), cv::FILLED);
      cv::line(img, cv::Point(int(start.first), int(start.second)),
              cv::Point(int(end.first), int(end.second)), cv::Scalar(0, 255, 0),
              LINE_WIDTH);
    }
    video.write(img);
    cv::imshow("TrojanMap", img);
    cv::waitKey(1);
  }
	video.release();
}
/**
 * GetPlotLocation: Transform the location to the position on the map
 * 
 * @param  {double} lat         : latitude 
 * @param  {double} lon         : longitude
 * @return {std::pair<double, double>}  : position on the map
 */
std::pair<double, double> TrojanMap::GetPlotLocation(double lat, double lon) {
  std::pair<double, double> bottomLeft(34.01001, -118.30000);
  std::pair<double, double> upperRight(34.03302, -118.26502);
  double h = upperRight.first - bottomLeft.first;
  double w = upperRight.second - bottomLeft.second;
  std::pair<double, double> result((lon - bottomLeft.second) / w * 1248,
                                   (1 - (lat - bottomLeft.first) / h) * 992);
  return result;
}

//-----------------------------------------------------
// TODO: Student should implement the following:
//-----------------------------------------------------
/**
 * GetLat: Get the latitude of a Node given its id.
 * 
 * @param  {std::string} id : location id
 * @return {double}         : latitude
 */
double TrojanMap::GetLat(std::string id) { 
    Node temp = data[id];     //Extracting the value for the corresponding key i.e. id from data map
  return temp.lat; 
}

/**
 * GetLon: Get the longitude of a Node given its id. 
 * 
 * @param  {std::string} id : location id
 * @return {double}         : longitude
 */
double TrojanMap::GetLon(std::string id) { 
  Node temp = data[id];  
  return temp.lon; 
}

/**
 * GetName: Get the name of a Node given its id.
 * 
 * @param  {std::string} id : location id
 * @return {std::string}    : name
 */
std::string TrojanMap::GetName(std::string id) { 
  Node temp = data[id];  
  return temp.name; 
}


std::string TrojanMap::GetId(std::string name) { 
  for(auto x=data.begin();x!=data.end();x++)
  {
    Node temp = x->second;
    if(temp.name == name)
    {
      return temp.id;
    }
  }
}

/**
 * GetNeighborIDs: Get the neighbor ids of a Node.
 * 
 * @param  {std::string} id            : location id
 * @return {std::vector<std::string>}  : neighbor ids
 */
std::vector<std::string> TrojanMap::GetNeighborIDs(std::string id) {
    std::vector<std::string> result;
    Node temp = data[id];
    return temp.neighbors;
}


/**
 * CalculateDistance: Get the distance between 2 nodes. 
 * 
 * @param  {Node} a  : node a
 * @param  {Node} b  : node b
 * @return {double}  : distance in mile
 */
double TrojanMap::CalculateDistance(const Node &a, const Node &b) {
  // TODO: Use Haversine Formula:
 
  double dlat = (b.lat - a.lat) * M_PI / 180.0;
  double dlon = (b.lon - a.lon) * M_PI / 180.0;

  // convert to radians
  double aLat = (a.lat) * M_PI / 180.0; //a is constant!!
  double bLat = (b.lat) * M_PI / 180.0;

  double a_ = pow(sin(dlat / 2), 2) + pow(sin(dlon / 2), 2) * cos(aLat) * cos(bLat);  
  double c = 2 * asin(sqrt(a_));
  double distances = 3961 * c;
  return distances;
}

/**
 * CalculatePathLength: Calculates the total path length for the locations inside the vector.
 * 
 * @param  {std::vector<std::string>} path : path
 * @return {double}                        : path length
 */
double TrojanMap::CalculatePathLength(const std::vector<std::string> &path) {
  double sum = 0;
  int prev_index = 0;
  int cur_index = 1;
  while(cur_index<path.size())
  {
    Node cur = data[path[cur_index]];
    Node prev = data[path[prev_index]];
    sum += CalculateDistance(cur,prev);                                                              //Calculate total distance between nodes
    prev_index = cur_index;
    cur_index += 1; //cur goes to next index inside path vector
  }
  return sum;
}





/**
 * Autocomplete: Given a parital name return all the possible locations with
 * partial name as the prefix.
 *
 * @param  {std::string} name          : partial name
 * @return {std::vector<std::string>}  : a vector of full names
 */
std::vector<std::string> TrojanMap::Autocomplete(std::string name) {
  std::vector<std::string> results{};
   std::string namecmp=name;
  std::string namesrc;


if (name=="")   return {"-1"};                                                 //Corner Case : when name is given blank. 
  
  for(auto i:nameVector)
    {
        std::string namesrc=i.substr(0,name.length());
        //npos returns -1. If substring is not found, find will return -1.
        //if substring is found, condition fails and count is incremented 
        //namesrc=i;
        transform(namesrc.begin(),namesrc.end(),namesrc.begin(),::tolower);   // Converting to lower for comparison 
        transform(namecmp.begin(),namecmp.end(),namecmp.begin(),::tolower);   // Converting to lower for comparison 

      //  std::cout<<namesrc<<" | | "<<namecmp<<std::endl;
        if (namecmp.compare(namesrc) == 0)
            results.push_back(i);                                 // Pushing original deta into the result vector
    }

  return results;
}

/**
 * GetPosition: Given a location name, return the position.
 *
 * @param  {std::string} name          : location name
 * @return {std::pair<double,double>}  : (lat, lon)
 */
std::pair<double, double> TrojanMap::GetPosition(std::string name) {
  std::pair<double, double> results(-1, -1);

  std::map<std::string, std::pair<double, double>>::iterator it = location_map.find(name);
  if (it != location_map.end())
    results=std::make_pair(it->second.first,it->second.second);                                 //Creating the result pair

  return results;
}


std::string TrojanMap::print_route(std::map<std::string,std::string> &parent_path,std::string i) {
  
  if(i == "null") return "";

  print_route(parent_path,parent_path[i]);
  //std::cout<<"i: "<<i<<std::endl;
  result.push_back(i);
  return i;
}



/**
 * CalculateShortestPath: Given 2 locations, return the shortest path which is a
 * list of id.
 *
 * @param  {std::string} location1_name     : start
 * @param  {std::string} location2_name     : goal
 * @return {std::vector<std::string>}       : path
 */



/*
  std::vector<std::string> TrojanMap::CalculateShortestPath(std::string location1_name,
                                                 std::string location2_name)
{
std::vector<std::string> x;
std::map<std::string, double> dist; //maps id with its respective distance from source

std::map<std::string, std::string> parent_path;
std::string start = GetId(location1_name);
std::string end = GetId(location2_name);


 std::map<std::string,std::vector<std::string>> adj_list;

 for (auto it: data){
   
          adj_list[it.second.id]= it.second.neighbors;

    }
    
  
  //for(auto i:adj_list)
  //{
  //  std::cout<<i.first;
  //  for (auto it:i.second)
  //  {
  //    std::cout<<it<<" ";
  //  }
  //  std::cout<<std::endl;
  //}
  

  typedef std::pair<double,std::string> dist_info;
  //                                                                                                            int V = data.size();

  std::priority_queue <dist_info, std::vector<dist_info>, std::greater<dist_info>> max_heap;


//  std::map<std::string,double> dist; //map of id and its distance from source

  for(auto it = data.begin(); it!=data.end();it++) {
    Node n = it->second;
    dist[n.id] = DBL_MAX;
  }


  max_heap.push(std::make_pair(0,GetId(location1_name)));

  parent_path[GetId(location1_name)]= "null"; // blank value for key source in previous map
  
  Node a = data[GetId(location1_name)]; // loc1 node
//  std::cout<<" loc1 id is: "<< GetId(location1_name)<<std::endl;

  std::string loc2_id= GetId(location2_name);
  //  std::cout<<" loc2 id is: "<< loc2_id<<std::endl;
  dist[a.id] = 0;
  double weight=0;

  std::string cur_node;

  while(!max_heap.empty())    //actually we should write while Max_heap->data != location2_name
  {
    cur_node = max_heap.top().second; // u is id of node on which we re currently present
    //x.push_back(u);
    max_heap.pop();

    //now we need an iterator to get the neighbours and their weights


  
    for(auto neighbor : adj_list[cur_node]) 
    {
        weight = CalculateDistance(data[cur_node],data[neighbor]);  //Shortest Distance between current node and neighbor 
    

    if(dist[neighbor] > (dist[cur_node] + weight)) //if distance of current node > distance of current node and it's neighbor
    {

    dist[neighbor] = (dist[cur_node] + weight); //Distance of source to current node + distance of current node to it's neighbor
    parent_path[neighbor] = cur_node; 
    max_heap.push(std::make_pair(dist[neighbor],neighbor));
    // x.push_back(cur_node);
    }
    }

  }
  
  //print_route
    
    for(auto i: data)
    {
      if(i.second.id==loc2_id && dist[i.second.id]!=DBL_MAX) 
      {
      std::cout<<(i.second.id==loc2_id && dist[i.second.id]!=DBL_MAX)<<" ";
      std::cout<<"Route is: ["<<print_route(parent_path,i.second.id);
      std::cout<<"]"<<std::endl;
      break;
      }
    }
   // x.push_back("Test");
   
   
   //for(auto i:result)
    //std::cout<<i<<" ";
  //std::cout<<std::endl;



   x = result;
  return x;
  }


*/






//BELMAN FORD ->

std::vector<std::string> TrojanMap::CalculateShortestPath(std::string location1_name,
                                                 std::string location2_name){
std::vector<std::string> x;
std::map<std::string,double> dist; //maps id with its respective distance from source

std::map<std::string, std::string> parent_path;
std::string start = GetId(location1_name);
std::string end = GetId(location2_name);

 std::map<std::string,std::vector<std::string>> adj_list;

 for (auto it: data){
   
          adj_list[it.second.id]= it.second.neighbors;

    }
 
 typedef std::pair<double,std::string> dist_info;                                                                                               int V = data.size();

  std::priority_queue <dist_info, std::vector<dist_info>, std::greater<dist_info>> max_heap;


//  std::map<std::string,double> dist; //map of id and its distance from source

  for(auto it = data.begin(); it!=data.end();it++) {
    Node n = it->second;
    dist[n.id] = DBL_MAX;
  }

    
  max_heap.push(std::make_pair(0,GetId(location1_name)));

  parent_path[GetId(location1_name)]= "null"; // blank value for key source in previous map
  
  Node a = data[GetId(location1_name)]; // loc1 node
//  std::cout<<" loc1 id is: "<< GetId(location1_name)<<std::endl;

  std::string loc2_id= GetId(location2_name);
  //  std::cout<<" loc2 id is: "<< loc2_id<<std::endl;
  dist[a.id] = 0;

  double weight=0;
  std::string cur_node;

  while(!max_heap.empty())    //actually we should write while Max_heap->data != location2_name
  {
    cur_node = max_heap.top().second; // u is id of node on which we re currently present
    //x.push_back(u);
    max_heap.pop();

    dist[cur_node]=0;

    for(auto neighbor : adj_list[cur_node]) 
    {
      weight = CalculateDistance(data[cur_node],data[neighbor]);  //Shortest Distance between current node and neighbor 
    
      for(auto inner_neighbor : adj_list[neighbor]) 
      {
     auto  weight_inner = CalculateDistance(data[cur_node],data[inner_neighbor]) +
             CalculateDistance(data[inner_neighbor],data[neighbor]);  //Shortest Distance between current node and neighbor 
         
         auto min_dist=std::min((dist[cur_node]+weight),(dist[cur_node]+weight_inner));

             if(dist[neighbor] > (dist[cur_node] + min_dist)) //if distance of current node > distance of current node and it's neighbor
              {

                dist[neighbor] = (dist[cur_node] + min_dist); //Distance of source to current node + distance of current node to it's neighbor
                parent_path[neighbor] = cur_node; 
                max_heap.push(std::make_pair(dist[neighbor],neighbor));
                // x.push_back(cur_node);
              }
      }
    }
  }

    for(auto i: data)
    {
      if(i.second.id==loc2_id && dist[i.second.id]!=DBL_MAX) 
      {
      std::cout<<(i.second.id==loc2_id && dist[i.second.id]!=DBL_MAX)<<" ";
      std::cout<<"Route is: ["<<print_route(parent_path,i.second.id);
      std::cout<<"]"<<std::endl;
      break;
      }
    }

 //x.push_back(start); 
//std::reverse(x.begin(),x.end());

   x = result;
  return x;

}





/**
 * Travelling salesman problem: Given a list of locations, return the shortest
 * path which visit all the places and back to the start point.
 *
 * @param  {std::vector<std::string>} input : a list of locations needs to visit
 * @return {std::pair<double, std::vector<std::vector<std::string>>} : a pair of total distance and the all the progress to get final path
 */
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravellingTrojan(
                                    std::vector<std::string> &location_ids) {
  std::pair<double, std::vector<std::vector<std::string>>> results;
  return results;
} 
