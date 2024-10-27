#include "PODRacerSetup.h"


PODRacerSetup::PODRacerSetup(ConfigManager* configManager) {
    this->configManager = configManager;


}

//------------------------------------------------------------------------------
// Default setup for TDataRC, receiver configuration
//  channels do not have default values
//------------------------------------------------------------------------------
void PODRacerSetup::setDefaultValues(const char* namespace_name, TDataRC* data) {

    Value v;
    v.i_val = data->gimbal_max;
    v.type = TYPE_INT;

    configManager->setKeyValue(namespace_name,EXTRACT_FIELD_NAME(data->gimbal_min),v );
    v.i_val = data->gimbal_max;
    v.type = TYPE_INT;
    configManager->setKeyValue(namespace_name,EXTRACT_FIELD_NAME(data->gimbal_max),v );

    v.i_val = data->gimbal_mid;
    v.type = TYPE_INT;
    configManager->setKeyValue(namespace_name,EXTRACT_FIELD_NAME(data->gimbal_mid),v );
}

// readValuesForTask für dataRC Task
void PODRacerSetup::readValuesForTask(const char* namespace_name, TDataRC* data) {
    KeyValuePair kvp = {
         EXTRACT_FIELD_NAME(data->gimbal_min), namespace_name
    };
    configManager->getKeyValue(kvp);
    data->gimbal_min = kvp.value.i_val;

    kvp = {
        EXTRACT_FIELD_NAME(data->gimbal_max), namespace_name
    };
    configManager->getKeyValue(kvp);
    data->gimbal_max = kvp.value.i_val;

    kvp = {
        EXTRACT_FIELD_NAME(data->gimbal_mid), namespace_name
        
    };
    configManager->getKeyValue(kvp);
    data->gimbal_mid = kvp.value.i_val;

}




//------------------------------------------------------------------------------
// Default setup for TDataRC, surface configuration
//
//------------------------------------------------------------------------------
void PODRacerSetup::setDefaultValues(const char* namespace_name, TDataOFlow* data) {

    uint8_t* buf;
    size_t bufLength;
    Value v;
    KeyValuePair kvp;
    v.blob_val.elements = ARRAY_ELEMENTS(data->biasRPY);
    kvp = {
        EXTRACT_FIELD_NAME(data->biasRPY),
        namespace_name,
        v
    };
    configManager->arrayToByteArray(
        data->biasRPY,                      // das Array was serialisiert werden soll
        sizeof(data->biasRPY),              // Anzahl Bytes von des Arrays
        ARRAY_ELEMENTS(data->biasRPY),       // Anzahl der Elemente im Array
        &kvp.value.blob_val.data,           // date werden hier als blob abgelegt
        &kvp.value.blob_val.length          // Anzahl Bytes des blobs
    );
    configManager->setKeyValue(kvp);

    v.blob_val.elements = ARRAY_ELEMENTS(data->pidPGain);
    kvp = {
        EXTRACT_FIELD_NAME(data->pidPGain),
        namespace_name,
        v
    };
    configManager->arrayToByteArray(
        data->pidPGain,                      // das Array was serialisiert werden soll
        sizeof(data->pidPGain),              // Anzahl Bytes von des Arrays
        ARRAY_ELEMENTS(data->pidPGain),       // Anzahl der Elemente im Array
        &kvp.value.blob_val.data,           // date werden hier als blob abgelegt
        &kvp.value.blob_val.length          // Anzahl Bytes des blobs
    );
    configManager->setKeyValue(kvp);

    v.blob_val.elements = ARRAY_ELEMENTS(data->pidRGain);
    kvp = {
        EXTRACT_FIELD_NAME(data->pidRGain),
        namespace_name,
        v
    };
    configManager->arrayToByteArray(
        data->pidRGain,                      // das Array was serialisiert werden soll
        sizeof(data->pidRGain),              // Anzahl Bytes von des Arrays
        ARRAY_ELEMENTS(data->pidRGain),       // Anzahl der Elemente im Array
        &kvp.value.blob_val.data,           // date werden hier als blob abgelegt
        &kvp.value.blob_val.length          // Anzahl Bytes des blobs
    );
    configManager->setKeyValue(kvp);

    v.blob_val.elements = ARRAY_ELEMENTS(data->pidYGain);
    kvp = {
        EXTRACT_FIELD_NAME(data->pidYGain),
        namespace_name,
        v
    };
    configManager->arrayToByteArray(
        data->pidYGain,                      // das Array was serialisiert werden soll
        sizeof(data->pidYGain),              // Anzahl Bytes von des Arrays
        ARRAY_ELEMENTS(data->pidYGain),       // Anzahl der Elemente im Array
        &kvp.value.blob_val.data,           // date werden hier als blob abgelegt
        &kvp.value.blob_val.length          // Anzahl Bytes des blobs
    );
    configManager->setKeyValue(kvp);


    v.blob_val.elements = ARRAY_ELEMENTS(data->setPointSlipRP);
    kvp = {
        EXTRACT_FIELD_NAME(data->setPointSlipRP),
        namespace_name,
        v
    };
    configManager->arrayToByteArray(
        data->setPointSlipRP,                      // das Array was serialisiert werden soll
        sizeof(data->setPointSlipRP),              // Anzahl Bytes von des Arrays
        ARRAY_ELEMENTS(data->setPointSlipRP),       // Anzahl der Elemente im Array
        &kvp.value.blob_val.data,           // date werden hier als blob abgelegt
        &kvp.value.blob_val.length          // Anzahl Bytes des blobs
    );
    configManager->setKeyValue(kvp);
}


//------------------------------------------------------------------------------
// Default setup for TDataSDist - SurfaceTask, surface configuration
//
//------------------------------------------------------------------------------
void PODRacerSetup::setDefaultValues(const char* namespace_name, TDataSurface* data) {
    // Value value;
    // value.i_val = data->pinTOF;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->pinTOF), value, TYPE_INT);
    // value.i_val = data->pinRXLidar;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->pinRXLidar), value, TYPE_INT);
    // value.i_val = data->pinTXLidar;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->pinTXLidar), value, TYPE_INT);

    // // ein Array wird als BLOB gespeichert
    // size = sizeof(data->pidTOF) / sizeof(data->pidTOF[0]);
    // ARRAY_TO_BLOB(data->pidTOF, blob, size, uint8_t);
    // value.s_val = blob;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->pidTOF), value, TYPE_BLOB);

    // size = sizeof(data->pidLIDAR) / sizeof(data->pidLIDAR[0]);
    // ARRAY_TO_BLOB(data->pidLIDAR, blob, size, uint8_t);
    // value.s_val = blob;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->pidLIDAR), value, TYPE_BLOB);

    // value.i_val = data->minHoverHeight;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->minHoverHeight), value, TYPE_INT);
    // value.i_val = data->maxHoverHeight;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->maxHoverHeight), value, TYPE_INT);
    // value.i_val = data->minHeight;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->minHeight), value, TYPE_INT);
    // value.i_val = data->maxHeight;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->maxHeight), value, TYPE_INT);

    // value.i_val = data->sensorConditionMin;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->sensorConditionMin), value, TYPE_INT);
    // value.i_val = data->sensorConditionMax;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->sensorConditionMax), value, TYPE_INT);

    // value.i_val = data->pidOutputError;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->pidOutputError), value, TYPE_INT);

    // value.i_val = data->thresholdRangeMM;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->thresholdRangeMM), value, TYPE_INT);
    // value.i_val = data->continousScanRate;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->pinTOF), value, TYPE_INT);

    // value.i_val = data->bias;
    // configManager->write(namespace_name, EXTRACT_FIELD_NAME(data->bias), value, TYPE_INT);

}



// readValuesForTask für dataOpticalFlow Task
void PODRacerSetup::readValuesForTask(const char* namespace_name, TDataOFlow* data) {
    KeyValuePair kvp;
    size_t size = ARRAY_ELEMENTS(data->biasRPY);
    Value v;
    kvp = {EXTRACT_FIELD_NAME(data->biasRPY), namespace_name, v};
    configManager->getKeyValue(kvp);
    configManager->byteArrayToArray(
        kvp.value.blob_val.data,
        kvp.value.blob_val.length,
        &data->biasRPY
    );

    kvp = {EXTRACT_FIELD_NAME(data->pidRGain), namespace_name, v};
    configManager->getKeyValue(kvp);
    configManager->byteArrayToArray(
        kvp.value.blob_val.data,
        kvp.value.blob_val.length,
        &data->pidRGain
    );

    kvp = {EXTRACT_FIELD_NAME(data->pidPGain), namespace_name, v};
    configManager->getKeyValue(kvp);
    configManager->byteArrayToArray(
        kvp.value.blob_val.data,
        kvp.value.blob_val.length,
        &data->pidPGain
    );

    kvp = {EXTRACT_FIELD_NAME(data->pidYGain), namespace_name, v};
    configManager->getKeyValue(kvp);
    configManager->byteArrayToArray(
        kvp.value.blob_val.data,
        kvp.value.blob_val.length,
        &data->pidYGain
    );

    kvp = {EXTRACT_FIELD_NAME(data->setPointSlipRP), namespace_name, v};
    configManager->getKeyValue(kvp);
    configManager->byteArrayToArray(
        kvp.value.blob_val.data,
        kvp.value.blob_val.length,
        &data->setPointSlipRP
    );
}
