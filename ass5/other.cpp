

//int choose(int n, int k){
    //if(k == 0) 
        //return 1;
    //return (n * choose(n - 1, k - 1)) / k;
//}

//int bezier(int i, int n, float u){
    //return choose(n, i) * std::pow(u,i) * std::pow(1-u,n-i);
//}

//Point calculate_bezier(float u, float v){
    //Point ret;
    //int n = controls.size()-1;
    //int m = controls[0].size()-1;
    //for(int i = 0; i < n; i++){
        //for(int j = 0; j < m; j++){
            //int b1 = bezier(i, n, u);
            //int b2 = bezier(j, m, v);
            //ret.x += b1*b2*controls[i][j].x;
            //ret.y += b1*b2*controls[i][j].y;
            //ret.z += b1*b2*controls[i][j].z;
        //}
    //}
    //return ret;
//}

//Point GetOGLPos(int x, int y)
//{
    //GLint viewport[4];
    //GLdouble modelview[16];
    //GLdouble projection[16];
    //GLfloat winX, winY, winZ;
    //GLdouble posX, posY, posZ;
 
    //glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    //glGetDoublev( GL_PROJECTION_MATRIX, projection );
    //glGetIntegerv( GL_VIEWPORT, viewport );
 
    //winX = (float)x;
    //winY = (float)viewport[3] - (float)y;
    //glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    //gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
 
    //return Point(posX, posY, posZ);
//}

//Point unproject(int x, int y){
    //GLdouble objx, objy, objz;
    //GLfloat winx, winy, winz;
    //GLint view[4];
    //GLdouble proj[16];
    //GLdouble model[16];

    //glGetDoublev(GL_MODELVIEW_MATRIX, model);
    //glGetDoublev(GL_PROJECTION_MATRIX, proj);
    //glGetIntegerv(GL_VIEWPORT, view);

    //winx = x;
    //winy = view[3]-y;
    //glReadPixels( x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winz);
    //gluUnProject(winx, winy, winz, model, proj, view, &objx, &objy, &objz);
    ////objx *= 
    ////objy *= 
    //return Point(objx, objy, objz);
//}

void draw_points(){
    glLineWidth(1);
    glPointSize(5);
    glColor3f(0,1,0);
    glBegin(GL_POINTS);
    for(auto i: points){
        for(Point p: i){
            glVertex3f(p.x*2, p.y*2, p.z*2);
        }
    }
    glEnd();
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    for(int i = 0; i < points.size(); i++){
        for(int j = 0; j < points[0].size()-1; j++){
            glVertex3f(points[i][j].x*2, points[i][j].y*2, points[i][j].z*2);
            glVertex3f(points[i][j+1].x*2, points[i][j+1].y*2, points[i][j+1].z*2);
        }
    }
    for(int i = 0; i < points.size(); i++){
        for(int j = 0; j < points[0].size()-1; j++){
            glVertex3f(points[j][i].x*2, points[j][i].y*2, points[j][i].z*2);
            glVertex3f(points[j+1][i].x*2, points[j+1][i].y*2, points[j+1][i].z*2);
        }
    }
    glEnd();
    glPointSize(1);
}
