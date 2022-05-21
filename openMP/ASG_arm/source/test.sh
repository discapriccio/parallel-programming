# test.sh
# !/bin/sh
pssh -h $PBS_NODEFILE  mkdir -p /home/s2012197/openmp/ASG_arm/source 1>&2
pscp -h $PBS_NODEFILE /home/s2012197/openmp/ASG_arm/source/34696.spi /home/s2012197/openmp/ASG_arm/source 1>&2
pscp -h $PBS_NODEFILE /home/s2012197/openmp/ASG_arm/source/a.out /home/s2012197/openmp/ASG_arm/source 1>&2
/home/s2012197/openmp/ASG_arm/source/a.out
