import json
import matplotlib.pyplot as plt
import numpy as np
from scipy import sparse
from scipy.sparse import csc_matrix

def agresti_coull_intetrval(pair):
    # num trial n
    success = pair[0]
    fail = pair[1]
    rate = success / (success + fail)
    n = success + fail
    z = 2
    n_tilda = n  + z ** 2
    p_tilda = (1 / n_tilda) * (success + (z**2/2) )
    dif = (z * (np.sqrt(  (p_tilda / n_tilda) * (1 - p_tilda) )))
    return dif

def read_jsons(json_file_names):
    data = []
    for i in json_file_names:
        curve = json_to_data(i)
        data.append(curve)
    return data

def read_jsons_withDFLE(json_file_names):
    data = []
    for i in json_file_names:
        curve = json_to_data_with_DFLE(i)
        data.append(curve)
    return data

def json_to_data(json_file_name):
    assignment = ['deterministic','random','row col']
    
    data = open(json_file_name)
    data = json.load(data)
    res = data['results']
    rate = data['success_rate']
    error = data['error_bar_for_success_rate']
    label = 'm=' + str(data['num_multiplexing']) + ' , ' + assignment[data['assignment type']] 
    return res,rate,error,label

def json_to_data_with_DFLE(json_file_name):
    assignment = ['deterministic','random','row col']
    
    data = open(json_file_name)
    data = json.load(data)
    results = data['results']
    success_rate = data['success_rate']
    error_bar_for_success_rate = data['error_bar_for_success_rate']
    DFrate = data['DFrate']
    DFerrors = data['DFerrors']
    nonDFLErate = data['nonDFLErate']
    erasure_rates = data['erasure_rates']
    
    label = 'm=' + str(data['num_multiplexing']) + 'assigntype=' + assignment[data['assignment type']] 
    return results,success_rate,error_bar_for_success_rate, label, DFrate, DFerrors, nonDFLErate, erasure_rates 

def plot_multiple_data(list_of_res, max_e, min_e, step,n,k,save = False, log =True):
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    
    phys_err = np.arange(min_e ,max_e, step)
    for i in list_of_res:
        ax.errorbar(phys_err, i[1], i[2], linewidth= 1, label = i[3])
    
    file_name = "[[" + str(n) + "," +str(k) + "]] LDPC HGP code"
    if log == True:
        ax.set_yscale('log')
    ax.set_title(file_name, fontsize = 15)
    ax.set_xlabel("photon loss probability", fontsize = 15)
    ax.set_ylabel(r"decoder failure & logical $X$ error prob", fontsize = 15)
    ax.set_xlim(min_e, max_e)
    plt.style.use('tableau-colorblind10')
    plt.legend(loc='lower right', fontsize = 13)
    if save == True:
        file_name = file_name + '.pdf'
        plt.savefig(file_name)
        
    ax.set_xlim([0,0.4])
    plt.show()
    return 0

def plot_multiple_data_with_DFLE(list_of_res, max_e, min_e, step,n,k,save = False, log =True, filename = None):
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)

    # phys_err = np.arange(min_e ,max_e, step)
    for i in list_of_res:       
        ax.errorbar(i[7], i[1], i[2], linewidth= 1, label = i[3]) # label = i[3]
        ax.errorbar(i[7], i[4], i[5], linewidth= 1, label = i[3] + ' DF')
        ax.plot(i[7], i[6], linewidth=1, label = i[3] + ' nonDF LE')
    # ax.set_xlim(min_e, max_e)
    ax.set_xlim(0, 0.4)
    
    if file_name == None:
        file_name = "[[" + str(n) + "," +str(k) + "]] LDPC HGP code" 
        
    if log == True:
        ax.set_yscale('log')
    ax.set_title(file_name, fontsize = 15)
    ax.set_xlabel("photon loss probability", fontsize = 15)
    ax.set_ylabel(r"decoder failure & logical $X$ error prob", fontsize = 15)
    plt.style.use('tableau-colorblind10')
    plt.legend(loc='lower right', fontsize = 13)
    if save == True:
        file_name = file_name + '.pdf'
        plt.savefig(file_name)
    plt.show()
    return 0