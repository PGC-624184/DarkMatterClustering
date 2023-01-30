static inline unsigned int find_path_compress(const unsigned int idx, CellParentStart *restrict cells)
{
  /*
    Find root (domain) with path compression (i.e. set all parents to final root 
  */

  unsigned int stack_used=0, stack[16], domain = cells[idx].parent;

  for(unsigned int child=idx;child!=domain;domain = cells[child=domain].parent)
    stack[stack_used++] = child;
  
#ifdef DEBUG
  if (stack_used>max_stack_usage)
    printf("Increasing stack used to %d\n",(max_stack_usage = stack_used));
#endif

  // Set all those paths to have final parent
  while (stack_used)
    cells[stack[--stack_used]].parent = domain;

  return domain;
}