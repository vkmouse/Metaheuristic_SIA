set dataset_path=dataset/DS1.txt
set max_evaluation_time=200000
set per_evaluation_time=4000
set num_run=2

set num_neighbor=7
set max_temperature=10
set min_temperature=0.00001
set decrease_rate=0.99

set num_cms=40
set Pc=0.6
set Pm=0.1

REM main.exe hc %dataset_path% %max_evaluation_time% %per_evaluation_time% %num_run%
REM main.exe sa %dataset_path% %max_evaluation_time% %per_evaluation_time% %num_run% %num_neighbor% %max_temperature% %min_temperature% %decrease_rate%
main.exe ga %dataset_path% %max_evaluation_time% %per_evaluation_time% %num_run% %num_cms% %Pc% %Pm% 