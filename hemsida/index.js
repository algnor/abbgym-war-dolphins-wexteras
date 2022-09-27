
let watering = false
let flakt = false

function vattna () {
    watering = !watering
    console.log(watering)

    fetch("settings?water=" + watering)
}

function fan () {
    flakt = !flakt
    console.log(flakt)

    fetch("settings?fan=" + (flakt ? 255 : 0))
}