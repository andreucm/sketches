// listpcm.c
// Lists the hardware names of wave output device/sub-devices
// upon each ALSA sound card in the system.
//
// Compile as:
// gcc -o listpcm listpcm.c -lasound

#include <stdio.h>
#include <string.h>
#include <alsa/asoundlib.h>

int main(int argc, char **argv)
{
   register int  err;
   int           cardNum;

   // Start with first card
   cardNum = -1;

   for (;;)
   {
      snd_ctl_t *cardHandle;

      // Get next sound card's card number. When "cardNum" == -1, then ALSA
      // fetches the first card
      if ((err = snd_card_next(&cardNum)) < 0)
      {
         printf("Can't get the next card number: %s\n", snd_strerror(err));
         break;
      }

      // No more cards? ALSA sets "cardNum" to -1 if so
      if (cardNum < 0) break;

      // Open this card's control interface. We specify only the card number -- not
      // any device nor sub-device too
      {
      char   str[64];

      sprintf(str, "hw:%i", cardNum);
      if ((err = snd_ctl_open(&cardHandle, str, 0)) < 0)
      {
         printf("Can't open card %i: %s\n", cardNum, snd_strerror(err));
         continue;
      }
      }

      {
      int      devNum;

      // Start with the first wave device on this card
      devNum = -1;
		
      for (;;)
      {
         snd_pcm_info_t  *pcmInfo;
         register int        subDevCount, i;

         // Get the number of the next wave device on this card
         if ((err = snd_ctl_pcm_next_device(cardHandle, &devNum)) < 0)
         {
            printf("Can't get next wave device number: %s\n", snd_strerror(err));
            break;
         }

         // No more wave devices on this card? ALSA sets "devNum" to -1 if so.
         // NOTE: It's possible that this sound card may have no wave devices on it
         // at all, for example if it's only a MIDI card
         if (devNum < 0) break;

         // To get some info about the subdevices of this wave device (on the card), we need a
         // snd_pcm_info_t, so let's allocate one on the stack
         snd_pcm_info_alloca(&pcmInfo);
         memset(pcmInfo, 0, snd_pcm_info_sizeof());

         // Tell ALSA which device (number) we want info about
         snd_pcm_info_set_device(pcmInfo, devNum);

         // Get info on the wave outs of this device
         snd_pcm_info_set_stream(pcmInfo, SND_PCM_STREAM_PLAYBACK);

         i = -1;
         subDevCount = 1;

         // More subdevices?
         while (++i < subDevCount)
         {
            // Tell ALSA to fill in our snd_pcm_info_t with info on this subdevice
            snd_pcm_info_set_subdevice(pcmInfo, i);
            if ((err = snd_ctl_pcm_info(cardHandle, pcmInfo)) < 0)
            {
               printf("Can't get info for wave output subdevice hw:%i,%i,%i: %s\n", cardNum, devNum, i, snd_strerror(err));
               continue;
            }

            // Print out how many subdevices (once only)
            if (!i)
            {
               subDevCount = snd_pcm_info_get_subdevices_count(pcmInfo);
               printf("\nFound %i wave output subdevices on card %i\n", subDevCount, cardNum);
            }

            // NOTE: If there's only one subdevice, then the subdevice number is immaterial,
            // and can be omitted when you specify the hardware name
            printf((subDevCount > 1 ? "    hw:%i,%i,%i\n" : "    hw:%i,%i\n"), cardNum, devNum, i);
         }
      }
      }

      // Close the card's control interface after we're done with it
      snd_ctl_close(cardHandle);
   }

   snd_config_update_free_global();
}

