#include <vector>
#include <fstream>
#include <sstream>

struct Object {
    struct Vertex{
       float x, y, z, a; 
    };

    struct Face{
       int a, b, c, d; 
    };
    std::vector<Vertex> vertices; 
    std::vector<Face> faces; 

    void read(std::string path){
        std::string id;
        std::string line;
        std::ifstream reading(path);
        while(std::getline(reading, line)){
            std::istringstream in(line);
            in >> id;
            if(id == "v"){
                Vertex v;
                float x, y, z, a; 
                in >> x;
                in >> y;
                in >> z;
                in >> a;
                v.x = x; v.y = y; v.z = z; v.a = a;
                vertices.push_back(v);
            }
            else if(id == "f"){
                Face f;
                int a, b, c, d; 
                in >> a;
                in >> b;
                in >> c;
                in >> d;
                f.a = a; f.b = b; f.c = c; f.d = d;
                faces.push_back(f);
            }
        }
    }

    void draw(){
        for(Face f: faces){
            glBegin(GL_LINE_LOOP);
            glVertex3f(vertices[f.a-1].x, vertices[f.a-1].y, vertices[f.a-1].z);
            glVertex3f(vertices[f.b-1].x, vertices[f.b-1].y, vertices[f.b-1].z);
            glVertex3f(vertices[f.c-1].x, vertices[f.c-1].y, vertices[f.c-1].z);
            glVertex3f(vertices[f.d-1].x, vertices[f.d-1].y, vertices[f.d-1].z);
            glEnd();
        }
    }
};
