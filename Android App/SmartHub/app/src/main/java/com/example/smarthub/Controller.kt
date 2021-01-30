package com.example.smarthub

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import java.util.*
import android.bluetooth.BluetoothSocket
import android.util.Log
import kotlinx.android.synthetic.main.controller.*
import java.io.IOException
import android.app.ProgressDialog
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.Context
import android.os.AsyncTask


class Controller: AppCompatActivity(){

    companion object {
        var m_myUUID: UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB") //giving a UUID for the session
        var m_bluetoothSocket: BluetoothSocket? = null
        lateinit var m_progress: ProgressDialog
        lateinit var m_bluetoothAdapter: BluetoothAdapter
        var m_isConnected: Boolean = false
        lateinit var m_address: String
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContentView(R.layout.controller)
        m_address = intent.getStringExtra(MainActivity.EXTRA_ADDRESS)

        ConnectToDevice(this).execute()

        lockBtn.setOnClickListener { sendCommand("1") } //when this btn is pressed send 1 to TIVA's bluetooth module
        unlockBtn.setOnClickListener { sendCommand("0") } //when this btn is pressed send 0 to TIVA's bluetooth module

        sound1.setOnClickListener { sendCommand("a.") } //when this btn is pressed send a to TIVA's bluetooth module
        sound2.setOnClickListener { sendCommand("b.") } //when this btn is pressed send b to TIVA's bluetooth module
        sound3.setOnClickListener { sendCommand("c.") } //when this btn is pressed send c to TIVA's bluetooth module
        sound4.setOnClickListener { sendCommand("d.") } //when this btn is pressed send d to TIVA's bluetooth module
        sound5.setOnClickListener { sendCommand("e.") } //when this btn is pressed send e to TIVA's bluetooth module
        sound6.setOnClickListener { sendCommand("f.") } //when this btn is pressed send f to TIVA's bluetooth module
        sound7.setOnClickListener { sendCommand("g.") } //when this btn is pressed send g to TIVA's bluetooth module
        sound8.setOnClickListener { sendCommand("h.") } //when this btn is pressed send h to TIVA's bluetooth module
        sound9.setOnClickListener { sendCommand("i.") } //when this btn is pressed send i to TIVA's bluetooth module
        sound10.setOnClickListener { sendCommand("j.") } //when this btn is pressed send j to TIVA's bluetooth module
        sound11.setOnClickListener { sendCommand("k.") } //when this btn is pressed send k to TIVA's bluetooth module

        volUp.setOnClickListener { sendCommand("+.") } //when this btn is pressed send + to TIVA's bluetooth module
        volDwn.setOnClickListener { sendCommand("-.") } //when this btn is pressed send - to TIVA's bluetooth module

        disconnect.setOnClickListener { disconnect() }

    }


    /*This function will send a string through bluetooth adapter to the connected device*/
    private fun sendCommand(input: String) {
        if (m_bluetoothSocket != null) {
            try{
                Log.d("sending",input);
                m_bluetoothSocket!!.outputStream.write(input.toByteArray())

            } catch(e: IOException) {
                e.printStackTrace()
            }
        }
    }

    /*This will disconnect currently connected device*/
    private fun disconnect() {
        if (m_bluetoothSocket != null) {
            try {
                m_bluetoothSocket!!.close()
                m_bluetoothSocket = null
                m_isConnected = false
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
        finish()
    }


    /*This is being called when the program jumps to new scene (i.e controller view), and connects to the selected device */
    private class ConnectToDevice(c: Context) : AsyncTask<Void, Void, String>() {
        private var connectSuccess: Boolean = true
        private val context: Context

        init {
            this.context = c
        }

        override fun onPreExecute() {
            super.onPreExecute()
            m_progress = ProgressDialog.show(context, "Coonecting to Smart Doorbell ...", "Please wait")
        }

        override fun doInBackground(vararg p0: Void?): String? {
            try {
                if (m_bluetoothSocket == null || !m_isConnected) {
                    m_bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
                    val device: BluetoothDevice = m_bluetoothAdapter.getRemoteDevice(m_address)
                    m_bluetoothSocket = device.createInsecureRfcommSocketToServiceRecord(m_myUUID)
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery()
                    m_bluetoothSocket!!.connect()
                }
            } catch (e: IOException) {
                connectSuccess = false
                e.printStackTrace()
            }
            return null
        }

        override fun onPostExecute(result: String?) {
            super.onPostExecute(result)
            if (!connectSuccess) {
                Log.i("data", "couldn't connect")
            } else {
                m_isConnected = true
            }
            m_progress.dismiss()
        }
    }



}