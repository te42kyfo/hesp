__kernel void update_positions( const unsigned int N, const real dt, global real * m,
								global real * px, global real * py, global real * pz,
								global real * vx, global real * vy, global real * vz,
								global real * fx, global real * fy, global real * fz,
								const real xmin, const real ymin, const real zmin,
								const real xmax, const real ymax, const real zmax) {


    const int globalid = get_global_id(0);
	if( globalid >= N ) return;


	px[globalid] += vx[globalid]*dt + 0.5*fx[globalid]*dt*dt / m[globalid];
	py[globalid] += vy[globalid]*dt + 0.5*fy[globalid]*dt*dt / m[globalid];
	pz[globalid] += vz[globalid]*dt + 0.5*fz[globalid]*dt*dt / m[globalid];

	if( px[globalid] >= xmax ) px[globalid] = xmin + px[globalid] - xmax;
	if( py[globalid] >= ymax ) py[globalid] = ymin + py[globalid] - ymax;
	if( pz[globalid] >= zmax ) pz[globalid] = zmin + pz[globalid] - zmax;

	if( px[globalid] <= xmin ) px[globalid] = xmax - xmin + px[globalid];
	if( py[globalid] <= ymin ) py[globalid] = ymax - ymin + py[globalid];
	if( pz[globalid] <= zmin ) pz[globalid] = zmax - zmin + pz[globalid];


}




__kernel void update_velocities( const unsigned int N,
								 const real dt, const real epsilon, const real sigma,
								 global real * m,
								 global real * px, global real * py, global real * pz,
								 global real * vx, global real * vy, global real * vz,
								 global real * fx, global real * fy, global real * fz,
								 global int* cells, global int* links,
								 const real xmin, const real ymin, const real zmin,
								 const real xmax, const real ymax, const real zmax,
								 const unsigned int nx,
								 const unsigned int ny,
								 const unsigned int nz,
								 const real r_cut) {

	const int globalid = get_global_id(0);
	if( globalid >= N ) return;

	real new_force_x = 0.0;
	real new_force_y = 0.0;
	real new_force_z = 0.0;

	const int xindex = (px[globalid]-xmin) / (xmax-xmin) * nx;
	const int yindex = (py[globalid]-ymin) / (ymax-ymin) * ny;
	const int zindex = (pz[globalid]-zmin) / (zmax-zmin) * nz;






	for( int iz = -1; iz <= 1; iz++) {
		for( int iy = -1; iy <= 1; iy++) {
			for( int ix = -1; ix <= 1; ix++) {

				int xlocal = xindex+ix;
				int ylocal = yindex+iy;
				int zlocal = zindex+iz;

				bool xwrapover = false;
				bool ywrapover = false;
				bool zwrapover = false;

				if( xlocal >= nx || xlocal < 0) xwrapover = true;
				if( ylocal >= ny || ylocal < 0) ywrapover = true;
				if( zlocal >= nz || zlocal < 0) zwrapover = true;

				xlocal %= nx;
				ylocal %= ny;
				zlocal %= nz;



				const int cellindex = zlocal * nx*ny + ylocal *nx + xlocal;
				int index = cells[cellindex];


				while( index != -1 ) {
					if( index != globalid ) {


						real dx = px[globalid] - px[index];
						real dy = py[globalid] - py[index];
						real dz = pz[globalid] - pz[index];

						if( xwrapover ) dx = (xmax-xmin) - dx;
						if( ywrapover ) dy = (ymax-ymin) - dy;
						if( zwrapover ) dz = (zmax-zmin) - dz;

						real sqlength = (dx*dx + dy*dy + dz*dz);
						if( sqlength <= r_cut*r_cut) {

							real sigma6 = sigma*sigma*sigma*sigma*sigma*sigma;

							real factor =
								24.0*epsilon/sqlength *
								sigma6 / (sqlength*sqlength*sqlength)*
								(2.0*sigma6 / (sqlength*sqlength*sqlength)-1.0);

							new_force_x += factor*dx;
							new_force_y += factor*dy;
							new_force_z += factor*dz;
						}
					}
					index = links[index];
				}
			}
		}
	}



	vx[globalid] += (fx[globalid] + new_force_x) * dt * 0.5 / m[globalid];
	vy[globalid] += (fy[globalid] + new_force_y) * dt * 0.5 / m[globalid];
	vz[globalid] += (fz[globalid] + new_force_z) * dt * 0.5 / m[globalid];

	fx[globalid] = new_force_x;
	fy[globalid] = new_force_y;
	fz[globalid] = new_force_z;

}

__kernel void reset_cells(  const unsigned int N,
							global int* cells) {
	const int globalid = get_global_id(0);
	if( globalid >= N ) return;

	cells[globalid] = -1;
}

__kernel void reset_links(  const unsigned int N,
							global int* links) {
	const int globalid = get_global_id(0);
	if( globalid >= N ) return;

	links[globalid] = globalid;
}

__kernel void update_cells( const unsigned int N,
							global int* cells, global int* links,
							global real* px, global  real* py, global real* pz,
							const real xmin, const real ymin, const real zmin,
							const real xmax, const real ymax, const real zmax,
							const unsigned int nx,
							const unsigned int ny,
							const unsigned int nz ) {
	const int globalid = get_global_id(0);
	if( globalid >= N ) return;

	const int xindex = (px[globalid]-xmin) / (xmax-xmin) * nx;
	const int yindex = (py[globalid]-ymin) / (ymax-ymin) * ny;
	const int zindex = (pz[globalid]-zmin) / (zmax-zmin) * nz;

	size_t cellindex = zindex * nx*ny + yindex*nx + xindex;


	links[globalid] = atomic_xchg(cells+cellindex, links[globalid]);


}
