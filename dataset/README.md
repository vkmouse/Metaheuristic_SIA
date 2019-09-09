### Outline

- [Description](#Description)
- [Compile](#Compile)
- [Usage](#Usage)
  - [For example](#For-example)

### Description
There are nine resource allocation problem datasets DS1 ~ DS9. Each of one has four parameters, number of resources, number of devices, number of interfaces, and number of services, respectively.


Dataset | number of resources | number of devices | number of interfaces | number of services 
------- |:-------------------:|:-----------------:|:--------------------:|:----------------
DS1     | 3                   | 5                 | 3                    | 100
DS2     | 3                   | 25                | 3                    | 100
DS3     | 3                   | 5                 | 3                    | 500
DS4     | 3                   | 25                | 3                    | 500
DS5     | 3                   | 50                | 3                    | 1,000
DS6     | 3                   | 50                | 3                    | 2,000
DS7     | 3                   | 75                | 3                    | 1,000
DS8     | 3                   | 75                | 3                    | 2,000
DS9     | 3                   | 100               | 3                    | 2,000

### Compile
    g++ create_dataset.cpp -o create_dataset.exe -O3

### Usage
Seven parameters are required to execute the program.

1. Number of interfaces $$/times$$ number of devices (I $$/times$$ Z)
2. Number of services (J)
3. Number of resources (K)
4. Minimum resources value
5. Maximum resources value
6. Minimum resources cost
7. Maximum resources cost


#### For example
Generate a dataset **dataset.txt** with number of interfaces $$/times$$ number of devices is 300, number of services is 2000, number of resources is 3, minimum resources value is 20, maximum resources value is 100, minimum resources cost is 5, and maximum resources cost is 40

    create_dataset.exe 300 2000 3 20 10 5 40 > dataset.txt