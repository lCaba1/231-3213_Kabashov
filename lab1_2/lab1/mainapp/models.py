from django.db import models

class Order(models.Model):
    user = models.IntegerField()
    base = models.CharField()
    quote = models.CharField()
    side = models.CharField()
    size = models.FloatField()
    price = models.FloatField()
    is_processed = models.BooleanField()
    opened_at = models.DateTimeField()
