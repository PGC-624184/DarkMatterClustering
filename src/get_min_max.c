void get_min_max(const double *pos, const uint32_t num_pos, double *restrict out) {
  /*
    Find the minimum and maximum (faster than numpy min & max calls)

    pos     - (num_pos,3) doubles of x,y,z
    num_pos - number of positions
    out     - 6 doubles for result of (min x,y,z max x,y,z)
   */

  if (!num_pos)
    return;

  double min_x = pos[0], max_x = min_x,
    min_y = pos[1], max_y = min_y,
    min_z = pos[2], max_z = min_z;

  
  for (size_t i=1;i<num_pos;i++)
    {
      if (pos[i*3]<min_x)
	min_x = pos[i*3];
      if (pos[i*3]>max_x)
	max_x = pos[i*3];

      if (pos[i*3+1]<min_y)
	min_y = pos[i*3+1];
      if (pos[i*3+1]>max_y)
	max_y = pos[i*3+1];

      if (pos[i*3+2]<min_z)
	min_z = pos[i*3+2];
      if (pos[i*3+2]>max_z)
	max_z = pos[i*3+2];

    }
  out[0] = min_x;
  out[1] = min_y;
  out[2] = min_z;
  out[3] = max_x;
  out[4] = max_y;
  out[5] = max_z;
}