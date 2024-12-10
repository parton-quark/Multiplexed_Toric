import json
import numpy as np
import matplotlib.pyplot as plt

def agresti_coull_intetrval(pair):
    # num trial n
    success = pair[0]
    fail = pair[1]
    rate = success / (success + fail)
    n = success + fail
    z = 2
    n_tilda = n  + z ** 2
    p_tilda = (1 / n_tilda) * (success + (z**2/2) )
    # conf_int_min = p_tilda + (z * (np.sqrt(  (p_tilda / n_tilda) * (1 - p_tilda) )))
    # conf_int_max = p_tilda - (z * (np.sqrt(  (p_tilda / n_tilda) * (1 - p_tilda) )))
    dif = (z * (np.sqrt(  (p_tilda / n_tilda) * (1 - p_tilda) )))
    # return [conf_int_min, rate, conf_int_max]
    # dif = conf_int_max - conf_int_min 
    return dif

def read_multiple_jsons(json_file_names):
    list_of_res = []
    for i in json_file_names:
        curve = json_to_data(i)
        list_of_res.append(curve)
    return list_of_res

def json_to_data(json_file_name):
    data = open(json_file_name)
    data = json.load(data)
    res = data['success_rate']
    
    rates = []
    errors = []
    
    for i in range(len(res)):
        num_success = res[i][0]
        num_fail = res[i][1]
        rate = num_fail / (num_success + num_fail)
        rates.append(rate)
        error = agresti_coull_intetrval([num_success, num_fail])
        errors.append(error)
    label = 'm=' + str(data['multiplexing']) + ' , strategy=' + str(data['strategy'])
    return res,rates,errors,label

def plot_multiple_data(list_of_res, phys_err, save=False):
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    
    for i in list_of_res:
        ax.errorbar(phys_err, i[1], i[2], linewidth= 1, label = i[3])
    ax.set_xlabel("photon loss probability", fontsize = 15)
    ax.set_ylabel(r"logical $Z$ error probability", fontsize = 15)
    plt.style.use('tableau-colorblind10')
    plt.legend(loc='lower right', fontsize = 13)
    if save == True:
        filename = "multiplexed_toric"
        plt.savefig(filename + ".pdf")
    plt.show()