
"use strict";

let MoveRobot = require('./MoveRobot.js')
let DeleteThermalSource = require('./DeleteThermalSource.js')
let AddCO2Source = require('./AddCO2Source.js')
let AddSoundSource = require('./AddSoundSource.js')
let DeleteRfidTag = require('./DeleteRfidTag.js')
let DeleteSoundSource = require('./DeleteSoundSource.js')
let LoadMap = require('./LoadMap.js')
let AddThermalSource = require('./AddThermalSource.js')
let LoadExternalMap = require('./LoadExternalMap.js')
let AddRfidTag = require('./AddRfidTag.js')
let DeleteCO2Source = require('./DeleteCO2Source.js')
let RegisterGui = require('./RegisterGui.js')

module.exports = {
  MoveRobot: MoveRobot,
  DeleteThermalSource: DeleteThermalSource,
  AddCO2Source: AddCO2Source,
  AddSoundSource: AddSoundSource,
  DeleteRfidTag: DeleteRfidTag,
  DeleteSoundSource: DeleteSoundSource,
  LoadMap: LoadMap,
  AddThermalSource: AddThermalSource,
  LoadExternalMap: LoadExternalMap,
  AddRfidTag: AddRfidTag,
  DeleteCO2Source: DeleteCO2Source,
  RegisterGui: RegisterGui,
};
