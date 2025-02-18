#!/usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import pandas as pd
import os 
import sys
from scipy.stats import binned_statistic_2d
import statistics as stats

from shutil import get_terminal_size
pd.set_option('display.width', get_terminal_size()[0]) 
pd.set_option('display.max_columns', None)

'''
infile = "DESI_trun-coords.dat-NED_tidy.dat--fluxes-fitted"; df = pd.read_csv(infile); print(df)
df = df.replace({'-999.00': np.nan}) # HOWEVER THUS APPEARS
NUV_lim = 10**((-(0.4*25.5)+3.56))*1e6 # MJy
FUV_lim = 10**((-(0.4*25.0)+3.56))*1e6 
dff = df.copy() 

dff['NUV_flag'] = 0
dff.loc[dff['FLUX_NUV'] == dff['FLUX_NUV'], 'NUV_flag'] = 1
dff['FUV_flag'] = 0
dff.loc[dff['FLUX_FUV'] == dff['FLUX_FUV'], 'FUV_flag'] = 1
dff.loc[dff['FLUX_NUV'] != dff['FLUX_NUV'], 'FLUX_NUV'] = NUV_lim
dff.loc[dff['FLUX_FUV'] != dff['FLUX_FUV'], 'FLUX_FUV'] = FUV_lim
##############################################################
dfq = df[df['class']== "QSO"] # QSOs ACCORDING TO NED
df_orig = df[['z', 'FLUX_G', 'FLUX_R', 'FLUX_Z', 'FLUX_W1', 'FLUX_W2']]
'''

infile = "DESI_trun.csv"; df = pd.read_csv(infile); #print(df) # TRY HIS
df.rename({'Z': 'z', 'b': 'Y'}, axis=1, inplace=True)
df_GRZW1W2 = df[['z', 'FLUX_G', 'FLUX_R', 'FLUX_Z', 'FLUX_W1', 'FLUX_W2']]
df_GRZ = df[['z', 'FLUX_G', 'FLUX_R', 'FLUX_Z']]

# dfQ = df[['z_NED', 'FLUX_G', 'FLUX_R', 'FLUX_Z', 'FLUX_W1', 'FLUX_W2','FLUX_NUV','FLUX_FUV']]
# dfQz = df[['z', 'FLUX_G', 'FLUX_R', 'FLUX_Z', 'FLUX_W1', 'FLUX_W2','FLUX_NUV','FLUX_FUV']] 
# dff = dff[['z','FLUX_G','FLUX_R','FLUX_Z','FLUX_W1','FLUX_W2','FLUX_NUV','FLUX_FUV','NUV_flag','FUV_flag']] 
# dfq = dfq[['z','FLUX_G','FLUX_R','FLUX_Z','FLUX_W1','FLUX_W2']]


def check_cols(D,col,what):
        #tmp = df[df[col] == what]; #print(tmp)
        tmp = D[pd.to_numeric(D[col], errors='coerce').isnull()]; print(tmp)
#check_cols(df,'FLUX_W2','n') # 201241  21123  2QZ  J140710.5-004915  

print(df_GRZW1W2.describe())# NOTHING MISSING


from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVR
from sklearn.neighbors import KNeighborsRegressor
from sklearn.tree import DecisionTreeRegressor
from sklearn.ensemble import RandomForestRegressor
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error 
from sklearn.metrics import mean_absolute_error 
from sklearn.metrics import r2_score
from scipy.stats import gaussian_kde  # FOR COLOR DENISTY PLOT
from matplotlib import cm
from scipy.stats import binned_statistic_2d

########## PLOT FUNCTION ##############
def update_ticks(z, pos):
    return "%1.0f" %(z)

from scipy.optimize import curve_fit
def origin_line(x,M):
        return M*x + 0 # FORCING A FIT THROUGH ZERO
def line(x,M,C):
        return M*x + C
    
from matplotlib.colors import LinearSegmentedColormap
from mpl_toolkits.axes_grid1 import make_axes_locatable
from matplotlib.colors import LogNorm
###### GET SOME STATS #####
def hist_stats(para):
    ave = np.mean(para)
    SD = np.std(para)
    return ave,SD
##########################
#cmap = plt.cm.viridis; col = 'r'
#cmap = plt.cm.gnuplot; col = 'g'
cmap = plt.cm.rainbow; col = 'firebrick'

def results_plot(data_label,key,y_test,y_pred):
        
    font = 14
    color_map = plt.cm.rainbow
    plt.rcParams.update({'font.size': font})
    fig, ax1 = plt.subplots(figsize=(7.5,5))
    
    plt.setp(ax1.spines.values(), linewidth=2)
    ax1.tick_params(direction='in', pad = 7,length=6, width=1.5, which='major', right=True, top = True)
    ax1.tick_params(direction='in', pad = 7,length=3, width=1.5, which='minor', right=True, top = True)
    ax1.tick_params(axis='both', which='major', labelsize=font)
    ax1.yaxis.set_major_locator(ticker.MultipleLocator(1)) # TICK SPACING
    ax1.xaxis.set_major_locator(ticker.MultipleLocator(1)) 
    ax1.xaxis.set_minor_locator(ticker.MultipleLocator(0.2))
    ax1.yaxis.set_minor_locator(ticker.MultipleLocator(0.2))   
        
    x1=0; x2=5.5; y1=0; y2=5.5
    
    x = y_test; y =  y_pred

    r2 = r2_score(y,x); dz = x - y; dzoverz = (x - y)/(x + 1);
    ave, rms = hist_stats(dz); ave, rms_norm = hist_stats(dzoverz)

    xplot = np.linspace(x1,x2,100); yplot = xplot; ltext = r"$z_{\rm spec} = z_{\rm phot}$"
    ax1.plot(xplot,yplot, ls = '--', color='b', lw=2,zorder=2,label= ltext)
        
    p0 = 1;  popt, pcov = curve_fit(origin_line,x,y,p0); C=0; ltext = 'Fit forced origin' 
    #p0 = [1,0]; popt, pcov = curve_fit(line,x,y,p0); C = popt[1]; ltext = 'Linear fit'

    xy = np.vstack([x,y]);z = gaussian_kde(xy)(xy); 
    scale = max(z)/min(z)
    h  = ax1.scatter(x,y,c=z*scale, s=5, cmap=cmap, norm=LogNorm(),zorder=1)
    #h = plt.hist2d(x, y, bins=100, cmap=cmap, norm=LogNorm(),zorder=1)
    cbar = plt.colorbar(h,ax=ax1, shrink=1,label = 'Number of sources',
                        aspect=24, pad=0.05,format=ticker.FuncFormatter(update_ticks))
    
    cbar.ax.tick_params(axis='y', direction='out',length=6, width=1.5, which = 'major')
    cbar.ax.tick_params(axis='y', direction='out',length=3, width=1.5, which = 'minor')

    ax1.set_xlim(x1,x2);ax1.set_ylim(y1,y2)
    x_pos = x1 + (x2-x1)/24; y_pos = y2 - (y2-y1)/12; yskip = (y2-y1)/16;
    text = "$n = %d, r = %1.3f, m (c = 0) = %1.2f$" %(len(x), abs(r2)**0.5, popt[0])
    ax1.text(x_pos,y_pos, text, color='k', size = 0.8*font)
    
    ax1.text(x_pos,y_pos-yskip, "$\sigma_{\Delta z} = %1.3f, \sigma_{\Delta z}/(z + 1) = %1.3f$" %(rms,rms_norm),color='k', size = 0.8*font)
    ##### OUTLIER FRACTION #######
    outliers = []
    def outlier(para):
        para = np.array(para)
        for i in range (0, len(dz)):
            if abs(para[i]) > rms:
                outliers.append(para)
        out_frac = 100*len(outliers)/len(para)
        return out_frac
    
    text = "%1.2f%% with $|\Delta z| > \sigma_{\Delta z}$" #, %1.2f%% with $|\Delta z/(z + 1)| > \sigma_{\Delta z}/(z + 1)$
    
    #ax1.text(x_pos,y_pos-2*yskip, text %(outlier(dz)), color='k', size = 0.8*font)
    ######## MAD #############
    def mad(para):
        para = np.array(para)
        return stats.median(para)

    MAD = 1.48*abs(mad(dz))
    NMAD = 1.48*abs(mad(dzoverz))
    ax1.text(x_pos,y_pos-2*yskip, r"$\sigma_{\rm MAD} = %1.4f, \sigma_{\rm NMAD} = %1.4f$" %(MAD,NMAD),color='k', size = 0.8*font)
    
    ## AFTER, ELSE SCATTER ENDS UP IN SUPLOT
    ax2 = fig.add_axes([0.5, 0.22, 0.25, 0.25]) # INSET left, bottom, width, height 
    plt.setp(ax2.spines.values(), linewidth=2)
    ax2.patch.set_alpha(0.5) # TRANSPARENT INSET
    ax2.set_xlim([-1.5,1.5])
    ax2.tick_params(direction='in', pad = 7,length=6, width=1.5, which='major')
    ax2.tick_params(direction='in', pad = 7,length=3, width=1.5, which='minor')
    ax2.tick_params(axis='both', pad = 5,which='major', labelsize=10)

    ax2.hist(x-y, bins=50, color="w", edgecolor='dimgrey', linewidth=2)
    
    ax2.axvline(x=rms, linestyle='dotted', color=col, lw=2)
    ax2.axvline(x=-rms, linestyle='dotted', color=col, lw=2)

    x1,x2 = ax2.get_xlim(); y1,y2 = ax2.get_ylim();
    x_pos = x2 - (x2-x1)/2.5; y_pos = y2 - (y2-y1)/8; yskip = (y2-y1)/16;
    #ax2.text(x_pos,y_pos, "$\sigma_{\Delta z}$ = %1.3f" %(rms),color='r', size = 0.7*font)

    ax1.set_xlabel(r'Spectroscopic redshift, $z_{\rm spec}$');
    ax1.set_ylabel(r'Predicted redshift, $z_{\rm phot}$')

    ax1.legend(fontsize = 0.8*font,loc='upper right',labelcolor='linecolor')

    plt.tight_layout()
    plot = '%s_%s_%s' %(infile,data_label,key);
    #ax2.set_rasterized(True) # TRANSPARENT   NOT WORKING, so
    #eps = "%s.eps" %(plot); plt.savefig(eps, format='eps',dpi=300);
    #plt.savefig(plot, format='pdf',dpi=300); print('Written to %s' %(eps))
    #os.system('pdftops %s %s.eps' %(plot,plot))
    png = "%s.png" %(plot); plt.savefig(png); print('Written to %s' %(png))
    plt.show();   plt.close()
    
############ ML/DL FUNCTION ###############
test_frac = 0.2; max_iter = 10000; cv = 10

def ML(data,Gn,target,blah,data_label):
    data = data[data > 0].dropna() # DROP NEGATIVE VALUES
    #print(data); print(data.describe());
    
    print("============== %s ==============" %(blah))
    target = target
    if Gn == 'y': # removing galaxex NaNs1
            data = data.dropna(subset = ['FLUX_NUV']) 
            data = data.dropna(subset = ['FLUX_FUV'])

    #print(data)
    df = data.sample(frac=1) # SHUFFLE
    print(df); print(df.describe());
    X = df.drop([target], axis = 1); y = df[target]; #print(X,y)

    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=test_frac, random_state=42)
    # SPLIT BEFORE NORMALISATION!
       
    X_to_scale=X_train.loc[:, ~X_train.columns.isin([target])]
    cols_to_scale = X_to_scale.columns; #print(cols_to_scale)
    scaler = StandardScaler()
    scaler.fit(data[cols_to_scale])
    X_train[cols_to_scale] = scaler.transform(X_train[cols_to_scale])
    X_test[cols_to_scale] = scaler.transform(X_test[cols_to_scale])
    #print(X_train)
    
    ### MACHINE LEARNING ###
    regressors = {
    "kNN": KNeighborsRegressor(n_neighbors = 27, weights='uniform'),
    #"SVR": SVR(C=2, gamma=0.1, epsilon=0.1), # TAKES WAY TOO LONG 
    #"DTR": DecisionTreeRegressor(max_depth =10),
    #"LR" : LinearRegression() # CRAP
}
    for key, model in regressors.items():
        model.fit(X_train, y_train)
        y_pred = model.predict(X_test) # making predictions
        rms = mean_squared_error(y_test, y_pred, squared=False)
        mae = mean_absolute_error(y_test, y_pred)
        r2 = r2_score(y_pred, y_test) # Coefficient of Determination

        print("For %s (%d out of %d sources), the RMS =  %1.3f MAE = %1.3f r = %1.3f"
              %(key,len(y_test),len(df),rms, mae,r2**0.5))

        results_plot(data_label,key, y_test, y_pred)

ML(df_GRZW1W2,'n','z','','df_GRZW1W2')
#ML(df_GRZ,'n','z','','df_GRZ')
