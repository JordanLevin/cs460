

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
