from tkinter import *
import numpy as np
from numpy.linalg import det

class Interface(Frame):

    """Notre fenêtre principale.
    Tous les widgets sont stockés comme attributs de cette fenêtre."""

    def __init__(self, fenetre, **kwargs):
        Frame.__init__(self, fenetre, width=800, height=800, **kwargs)
        self.pack(fill=BOTH)
        self.nb_clic = 0

        # Création de nos widgets
        #self.message = Label(self, text="Vous n'avez pas cliqué sur le bouton.")
        #self.message.pack()
##########ZONE DES Sliders######
        self.yaw = Scale(self, from_=0, to=256,label='yaw')
        self.yaw.bind("<ButtonRelease-1>", self.updateValue)
        self.yaw.pack(side="left")
        self.yaw.set(128)

        self.pitch = Scale(self, from_=0, to=256,label='pitch')
        self.pitch.bind("<ButtonRelease-1>", self.updateValue)
        self.pitch.pack(side="left")
        self.pitch.set(128)
        self.roll = Scale(self, from_=0, to=256,label='roll')
        self.roll.bind("<ButtonRelease-1>", self.updateValue)
        self.roll.pack(side="left")
        self.roll.set(128)
        self.throttle = Scale(self, from_=0, to=256,label='throttle')
        self.throttle.bind("<ButtonRelease-1>", self.updateValue)
        self.throttle.pack(side="left")


        self.k = Scale(self, from_=0, to=1,resolution=0.01,label='k')
        self.k.bind("<ButtonRelease-1>", self.updateValue)
        self.k.pack(side="left")
        self.k.set(1)
#########ZONE DE TEXT#######
        self.yawValue = Label(self, text="yaw ="+str(self.yaw.get()))
        self.yawValue.pack(side="top")

        self.pitchValue = Label(self, text="pitch ="+str(self.pitch.get()))
        self.pitchValue.pack(side="top")

        self.rollValue = Label(self, text="roll ="+str(self.roll.get()))
        self.rollValue.pack(side="top")

        self.throttleValue = Label(self, text="throttle ="+str(self.throttle.get()))
        self.throttleValue.pack(side="top")

        self.kValue = Label(self, text="K ="+str(self.k.get()))
        self.kValue.pack(side="top")

        self.value = Label(self, text="void")
        self.value.pack(side="left")

        self.bouton_quitter = Button(self, text="Quitter", command=self.quit)
        self.bouton_quitter.pack(side="right")

        #self.bouton_cliquer = Button(self, text="Cliquez ici", fg="red",
        #        command=self.cliquer)
        #self.bouton_cliquer.pack(side="right")


    def updateValue(self, event):
        self.yawValue["text"]= "yaw = {}".format(self.yaw.get())
        self.pitchValue["text"]= "pitch = {}".format(self.pitch.get())
        self.rollValue["text"]= "roll = {}".format(self.roll.get())
        self.throttleValue["text"]= "throttle = {}".format(self.throttle.get())
        self.kValue["text"]= "K = {}".format(self.k.get())
        val=calcul(self.roll.get(),self.pitch.get(),self.yaw.get(), self.throttle.get(), self.k.get())
        self.value["text"]= val


def calcul(roll, pitch,yaw, throttle,k):
    #mot1= np.array([roll,-k,-k,k], [pitch,k,-k,-k],[yaw,-k,k,-k],[throttle,k,k,k])
    #print(mot1)

    #mot1 = np.array([[roll,-k,-k,k], [pitch,k,-k,-k], [yaw,-k,k,-k], [throttle,k,k,k]])
    #mot2 = np.array([[-k,roll,-k,k], [k,pitch,-k,-k], [-k,yaw,k,-k], [k,throttle,k,k]])
    #mot3 = np.array([[-k,-k,roll,k], [-k,k,pitch,-k], [k,k,yaw,-k], [k,k,throttle,k]])
    #mot4 = np.array([[k,-k,-k,roll], [-k,k,-k,pitch], [-k,-k,k,yaw], [k,k,k,throttle]])
    mot1 = np.array([[roll,-k,-k,k], [pitch,k,-k,-k], [yaw,-k,k,-k], [throttle,k,k,k]])
    mot2 = np.array([[k,roll,-k,k], [k,pitch,-k,-k], [k,yaw,k,-k], [k,throttle,k,k]])
    mot3 = np.array([[k,-k,roll,k], [k,k,pitch,-k], [k,-k,yaw,-k], [k,k,throttle,k]])
    mot4 = np.array([[k,-k,-k,roll], [k,k,-k,pitch], [k,-k,k,yaw], [k,k,k,throttle]])



    #print(mot1)
    #print("mot2")
    #print(mot2)
    #print("mot3")
    #print(mot3)
    #print("mot4")
    #print(mot4)
    #print("------------------")
    valMot1= det(mot1)/(16*k)
    print("Mot1")
    print(valMot1)

    valMot2= det(mot2)/(16*k)
    print("Mot2")
    print(valMot2)

    valMot3= det(mot3)/(16*k)
    print("Mot3")
    print(valMot3)

    valMot4= det(mot4)/(16*k)
    print("Mot4")
    print(valMot4)
    print("---------------------")
    #print("mot1="+str(valMot1))
    #print("mot2="+str(valMot2))
    #print("mot3="+str(valMot3))
    #print("mot4="+str(valMot4))
    return "Mot1 = "+ str((float(valMot1)))[0:8]+" , Mot2 = "+str((float(valMot2)))[0:8]+" , Mot3 = "+str((float(valMot3)))[0:8]+" , Mot4 = "+str((float(valMot4)))[0:8]


    #def cliquer(self):

    #   self.nb_clic += 1
    #   self.message["text"] = "Vous avez cliqué {} fois.".format(self.nb_clic)

fenetre = Tk()
interface = Interface(fenetre)

interface.mainloop()
interface.destroy()
