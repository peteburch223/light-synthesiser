#ifndef _Duration_h
  #define _Duration_h

  struct Duration {
  	unsigned long duration;
  	unsigned long t2_prescaler_pointer;
  	unsigned long t2_comparator;
  	unsigned long t1_comparator;
  };
#endif
