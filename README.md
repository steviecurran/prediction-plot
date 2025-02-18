# prediction-plot

Code to performs machine learning (k-nearest neighbours regression) and plot the predicted versus measured values. This is all contained within kNN.py and supersedes the all of the C code above (thus not requiring me to write my own grey-scale function, etc.).

![](https://raw.githubusercontent.com/steviecurran/prediction-plot/refs/heads/main/DESI_trun.csv_df_GRZW1W2_kNN.png)

## Previous C version

Useful for large datasets, as produces a grey scale and error bars of equally binned data instead of individal points. 

Also returns a sub-plot showing the distribution of the difference in the predicted and measured values and gives the mean difference, the standard deviation and outlier fraction.

![](https://raw.githubusercontent.com/steviecurran/prediction-plot/refs/heads/main/cmp22-Fig9.png)
