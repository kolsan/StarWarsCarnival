#!/usr/bin/env python
# -*- coding: utf-8 -*-
#the next line is only needed for python2.x and not necessary for python3.x
from __future__ import print_function, division

import serial
import time
import pygame
import os
import sys
import random
# if using python2, the get_input command needs to act like raw_input:
if sys.version_info[:2] <= (2, 7):
    get_input = raw_input
else:
    get_input = input # python3
pygame.mixer.pre_init(44100, -16, 2, 2048) # setup mixer to avoid sound lag

pygame.init()
screen = pygame.display.set_mode((800, 600))
clock = pygame.time.Clock()
start_time = pygame.time.get_ticks()

# look for sound & music files in subfolder 'data'
#pygame.mixer.music.load(os.path.join('data', 'an-turr.ogg'))#load music
_songs = ['data\M_chewbacca.ogg','data\M_imperial_march.ogg','data\M_R2D2_Again.ogg','data\M_rebel-theme.ogg','data\M_star-wars-cantina-song.ogg','data\M_star-wars-theme-song.ogg','data\M_darth_vader_respirando.ogg']
_currently_playing_song = None
saberup = pygame.mixer.Sound(os.path.join('data','saberdown.wav'))  #load sound
fail = pygame.mixer.Sound(os.path.join('data','fail.wav'))  #load sound
jump = pygame.mixer.Sound(os.path.join('data','jump.wav'))  #load sound
NOK_001_002_003 = pygame.mixer.Sound(os.path.join('data','NOK_001_002_003.ogg'))  #load sound
NOK_004_005 = pygame.mixer.Sound(os.path.join('data','NOK_004_005.ogg'))  #load sound
OK_001_002_003 = pygame.mixer.Sound(os.path.join('data','OK_001_002_003.ogg'))  #load sound
OK_004_005 = pygame.mixer.Sound(os.path.join('data','OK_004_005.ogg'))  #load sound
UP_001_002_003 = pygame.mixer.Sound(os.path.join('data','UP_001_002_003.ogg'))  #load sound
UP_004_005 = pygame.mixer.Sound(os.path.join('data','UP_004_005.ogg'))  #load sound
SONG_END = pygame.USEREVENT + 1
pygame.mixer.music.set_endevent(SONG_END)
#ser = serial.Serial('/dev/ttyACM0', 9600,timeout=0)

#Variable
playing = False
done = False
f = 0
i = 0
LIME = (0,255,0)
gold = (255,215,0)
black = (0,0,0)
texto = ""
font = pygame.font.SysFont("comicsansms", 256)
font2 = pygame.font.SysFont("comicsansms", 64)
text = font.render(texto + str(f), True, gold)
gametime = 0
tiempo = ""

def play_a_different_song():
    global _currently_playing_song, _songs
    next_song = random.choice(_songs)
    while next_song == _currently_playing_song:
        next_song = random.choice(_songs)
    _currently_playing_song = next_song
    pygame.mixer.music.load(next_song)
    pygame.mixer.music.play()

while not done:

    #print (gametime)
    #print (time.time())
    if (time.time () > gametime) and playing :
        playing = False
        #gametime = 0
        print ("End of the game!!")
        print (gametime)
        print (time.time())
        pygame.mixer.music.stop()
        #ser.write ('2\n')
    if playing:
        tiempo = str(gametime - time.time())

        #print (gametime - time.time())
#       message = ser.readline()
        message = "hhhh"
        if message !="":
            #print(message + "\n")
            #print(message[0:5] + "\n")
            if message[0:5] == "go001":
                UP_001_002_003.play()
            if message[0:5] == "go002":
                UP_001_002_003.play()
            if message[0:5] == "go003":
                UP_001_002_003.play()
            if message[0:5] == "go004":
                UP_004_005.play()
            if message[0:5] == "go005":
                UP_004_005.play()
            if message[0:6] == "nok001":
                NOK_001_002_003.play()
            if message[0:6] == "nok002":
                NOK_001_002_003.play()
            if message[0:6] == "nok003":
                NOK_001_002_003.play()
            if message[0:6] == "nok004":
                NOK_004_005.play()
            if message[0:6] == "nok005":
                NOK_004_005.play()
            if message[0:5] == "ok001":
                OK_001_002_003.play()
                f = f + 10
            if message[0:5] == "ok002":
                OK_001_002_003.play()
                f = f + 10
            if message[0:5] == "ok003":
                OK_001_002_003.play()
                f = f + 10
            if message[0:5] == "ok004":
                OK_004_005.play()
                f = f + 20
            if message[0:5] == "ok005":
                OK_004_005.play()
                f = f + 20

    for event in pygame.event.get():
        if event.type == SONG_END:
            play_a_different_song()
            pygame.mixer.music.set_volume (0.2)
        if event.type == pygame.QUIT:
            done = True

        if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
            done = True
        pressed = pygame.key.get_pressed()
        if pressed[pygame.K_q]: done = True
        if pressed[pygame.K_s]:
#		    ser.write ('1\n')
            f= 0
            playing = True
            play_a_different_song()
            pygame.mixer.music.set_volume (1.0)
            pygame.time.delay(2000)
            pygame.mixer.music.set_volume (0.9)
            pygame.time.delay(2000)
            pygame.mixer.music.set_volume (0.7)
            pygame.time.delay(2000)
            pygame.mixer.music.set_volume (0.5)
            pygame.time.delay(2000)
            pygame.mixer.music.set_volume (0.4)
            pygame.time.delay(2000)
            pygame.mixer.music.set_volume (0.2)
            gametime = time.time() + 120
            #print (gametime - time.time())
            #print (gametime)
            #print (time.time())
            print ("Start!!!")
#-------------------------------------------
# Koldo atencion a esta linea
#--------------------------------------
        if pressed[pygame.K_e]:
            playing = False
            gametime = 0
            tiempo = ""
            pygame.mixer.music.stop()
            print ("End of the game!!")
            #ser.write ('2\n')

#	    if pressed[pygame.K_u]: ser.write ('3\n')
#	    if pressed[pygame.K_d]: ser.write ('4\n')
#	    if pressed[pygame.K_m]: ser.write ('5\n')
#	    if pressed[pygame.K_r]: ser.write ('6\n')

    screen.fill(black)
    if playing:
        counting_text = font2.render(tiempo[:3], 1, gold)

        if ( gametime - time.time () < 100):counting_text = font2.render(tiempo[:2], 1, gold)
        counting_rect = counting_text.get_rect(center = screen.get_rect().center)
        screen.blit(counting_text, (20,20))



    text = font.render(texto + str(f), True, gold)
    screen.blit(text,
        (320 - text.get_width() // 2, 240 - text.get_height() // 2))
    #if playing:
    pygame.display.flip()
    clock.tick(60)
    #i = i + 1
    #print (i)
    #if counting_time >= 100000: done = True

pygame.quit ()

