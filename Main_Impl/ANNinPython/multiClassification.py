import tensorflow as tf
from keras.models import load_model




def predict_class(loaded_model, features):
    fstring = features.split(',')
    #remove last element which is a comma since the features recieved has unneeded comma at the end
    del fstring[-1]
    features = [float(ele) for ele in fstring]
    prediction_features = loaded_model.predict(tf.expand_dims(features, axis=0))
    rate_prediction = max(prediction_features[0])
    prediction_rates = []
    for i in prediction_features[0]:
        prediction_rates.append(i)
    classified_class = prediction_rates.index(rate_prediction)
    return classified_class

def load_object(modelfile):
    loaded_model = load_model(modelfile)
    return loaded_model
