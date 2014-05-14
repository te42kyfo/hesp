__kernel void update_positions( const unsigned int N, const real dt, global real * m,
			  	     global real * px, global real * py, global real * pz,
			  	     global real * vx, global real * vy, global real * vz,
				     global real * fx, global real * fy, global real * fz ) {


    const int globalid = get_global_id(0);
	if( globalid >= N ) return;


	px[globalid] += vx[globalid]*dt + fx[globalid]*dt*dt / m[globalid];
	py[globalid] += vy[globalid]*dt + fy[globalid]*dt*dt / m[globalid];
	pz[globalid] += vz[globalid]*dt + fz[globalid]*dt*dt / m[globalid];

}



__kernel void update_velocities( const unsigned int N, const real epsilon, const real sigma,
								 global real * m,
								 global real * px, global real * py, global real * pz,
								 global real * vx, global real * vy, global real * vz,
								 global real * fx, global real * fy, global real * fz ) {

    const int globalid = get_global_id(0);
	if( globalid >= N ) return;

	real new_force_x = 0;
	real new_force_y = 0;
	real new_force_z = 0;

	for( unsigned int i = 0; i < N; i++) {
		if( i == globalid ) continue;

		real dx = px[globalid] - px[i];
		real dy = py[globalid] - py[i];
		real dz = pz[globalid] - pz[i];

		real invsqlength = 1.0/ (dx*dx + dy*dy + dz*dz);
		real sigma6 = sigma*sigma*sigma;
		sigma6 = sigma6*sigma6;

		real factor =
			24.0*epsilon*invsqlength *
			sigma6*invsqlength*invsqlength*invsqlength*
			(2.0*sigma6*invsqlength*invsqlength*invsqlength-1);

		new_force_x += factor*dx;
		new_force_y += factor*dy;
		new_force_z += factor*dz;

	}

	vx[globalid] += (fx[globalid] + new_force_x) * dt * 0.5 / m[globalid];
	vy[globalid] += (fy[globalid] + new_force_y) * dt * 0.5 / m[globalid];
	vz[globalid] += (fz[globalid] + new_force_z) * dt * 0.5 / m[globalid];

	fx[globalid] = new_force_x;
	fy[globalid] = new_force_y;
	fz[globalid] = new_force_z;





}