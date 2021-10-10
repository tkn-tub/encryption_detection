


import tensorflow as tf
from keras.models import load_model



def predict_class(loaded_model, features):

    fstring = features.split(',')

    del fstring[-1]
    features = [float(ele) for ele in fstring]


    prediction_features = loaded_model.predict(tf.expand_dims(features, axis=0))
    # print("Predictions")
    # print(prediction_features)
    if prediction_features[0][0] > 0.5:
        prediction = 0
    else:
        prediction = 1
    # print(prediction)
    return prediction


def load_object(modelfile):
    loaded_model = load_model(modelfile)
    return loaded_model

