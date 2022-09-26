

// function knapp () {
//     alert("jag kan släcka din heta eld med min vätska ;)")
// }

let btnPrimary = document.querySelector('#button')

let watering = false
let flakt = false

btnPrimary.addEventListener('click', () => btnPrimary.getElementsByClassName.backgroundColor = '#337ab7')

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