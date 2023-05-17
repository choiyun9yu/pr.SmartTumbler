# Aqu4Men Project

## Overview
Modern individuals are aware of the importance of sufficient hydration, but find it difficult to adhere to.
So we tried to create an environment where you drink water through a smart tumblr with AI.


## Text Mining
Fist of all, we used Web-crawling and Text-mining in order to research the tumblr market.
We collected tumblr purchase reviews and inputed into KoNLPy's OKT which is a morpheme separator.
And then, we created a Word-Cloud and TF-iDF vector.

![TextMining](https://github.com/choiyun9yu/pr.Aqu4Men/blob/main/data/textmining.png)


## Machin Learning
This model collects the angle of the tumbler through an acceleration sensor, uses it as feature data, and calculates the amount of water withdrawn from the tumbler as label data.

### Data Collection 
The data collection process involves the following steps. Firstly, the measurement values from the acceleration sensor are transmitted to the server using the EPS8266 module. Subsequently, these values are stored in the server's database.  
  
![Pipeline](https://github.com/choiyun9yu/pr.Aqu4Men/blob/main/data/pipeline.png)  
[![MariaDB](https://github.com/choiyun9yu/pr.Aqu4Men/blob/main/data/mariadb.svg)](https://github.com/choiyun9yu/pr.Aqu4Men/blob/main/Database/DB.SQL)

### Model Evaluation
#### Regressor
![Regressor](https://github.com/choiyun9yu/pr.Aqu4Men/blob/main/data/regressor.png)
#### Clustering
![Clustering](https://github.com/choiyun9yu/pr.Aqu4Men/blob/main/data/clustering.png)
  
*If you want to know about more detailed process [here](https://github.com/choiyun9yu/pr.Aqu4Men/blob/main/MachineLearning.ipynb).*

## Report
_**[A Flow Rate Prediction of Regression Models Using an Acceleration Sensor](https://github.com/choiyun9yu/pr.Aqu4Men/blob/main/data/report.pdf)**_

