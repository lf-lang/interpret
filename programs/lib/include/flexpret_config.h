
/**
  * This flexpret core config file is auto-generated, and based on the
  * configuration used to build the flexpret emulator.
  *
  * Do not edit.
  *
  */
#ifndef FLEXPRET_CONFIG_H
#define FLEXPRET_CONFIG_H

/* Memory ranges */
#define ISPM_START      0x00000000
#define ISPM_END        0x8000
#define ISPM_SIZE_KB    24
#define DSPM_START      0x20000000
#define DSPM_END        0x20000000 + 0x8000
#define DSPM_SIZE_KB    24
#define BUS_START       0x40000000
#define BUS_END         0x40000000 + 0x400

/* Scheduling */
#define SCHED_ROUND_ROBIN
#define NUM_THREADS     4

/* Timing */
#define CLOCK_PERIOD_NS 10
#define TIME_BITS       32

/* IO */
#define NUM_GPIS        4
#define GPI_SIZES       {1,1,1,1}
#define NUM_GPOS        4
#define GPO_SIZES       {2,2,2,2}

#endif
    