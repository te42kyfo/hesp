__kernel void force( const unsigned int N, global real * m,
			  	     global real * px, global real * py, global real * pz,
				     global real * fx, global real * fy, global real * fz ) {

    const int globalid = get_global_id(0);
    fx[globalid] = px[globalid]*2;
}
