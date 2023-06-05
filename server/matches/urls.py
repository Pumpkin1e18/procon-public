# -*- coding: utf-8 -*-
from django.urls import path
from . import views

urlpatterns = [
    path('', views.get_info),
    path('<int:matchID>/', views.get_state),
    path('<int:matchID>/action/', views.post_action),
]

